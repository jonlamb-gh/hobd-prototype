/**
 * @file gps.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <math.h>

#include "board.h"
#include "libsbp/sbp.h"
#include "libsbp/system.h"
#include "libsbp/navigation.h"
#include "uart_lib.h"
#include "uart_drv.h"
#include "hobd.h"
#include "ring_buffer.h"
#include "time.h"
#include "canbus.h"
#include "gps.h"




// *****************************************************
// static global types/macros
// *****************************************************

// GPS is on UART1
#define UART_RX_INTERRUPT USART1_RX_vect
#define UART_UCSRA UCSR1A
#define UART_UCSRB UCSR1B
#define UART_UCSRC UCSR1C
#define UART_DATA UDR1


//
#define gps_uart_enable() (UART_UCSRB |= (_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))


//
#define gps_uart_disable() (UART_UCSRB &= ~(_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))




// *****************************************************
// static global data
// *****************************************************

// UART rx ring buffer
static volatile ring_buffer_s rx_buffer;


// SBP state
static sbp_state_t sbp_state;


// SBP callback nodes
static sbp_msg_callbacks_node_t heartbeat_callback_node;
static sbp_msg_callbacks_node_t gps_time_node;
static sbp_msg_callbacks_node_t dops_node;
static sbp_msg_callbacks_node_t pos_llh_node;
static sbp_msg_callbacks_node_t baseline_ned_node;
static sbp_msg_callbacks_node_t vel_ned_node;
static sbp_msg_callbacks_node_t heading_node;




// *****************************************************
// static declarations
// *****************************************************

//
static void hw_init( void );


// prototype defined by libsbp
static uint32_t sbp_read_function(
        uint8_t *buff,
        uint32_t buff_len,
        void *context );


// callback prototypes are defined by libsbp
static void heartbeat_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void gps_time_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void dops_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void pos_llh_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void baseline_ned_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void vel_ned_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void heading_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static uint8_t publish_group_a(
        gps_state_s * const gps_state );


//
static uint8_t publish_group_b(
        gps_state_s * const gps_state );


//
static uint8_t publish_group_c(
        gps_state_s * const gps_state );


//
static uint8_t publish_group_d(
        gps_state_s * const gps_state );


//
static uint8_t publish_group_e(
        gps_state_s * const gps_state );


//
static uint8_t publish_group_f(
        gps_state_s * const gps_state );




// *****************************************************
// static definitions
// *****************************************************

//
ISR( UART_RX_INTERRUPT )
{
    // read UART status register and UART data register
    const uint8_t status  = UART_UCSRA;
    const uint8_t data = UART_DATA;

    // read error status
    rx_buffer.error = (status & (_BV(FE1) | _BV(DOR1)) );

    // push data into the rx buffer, error is updated with return status
    (void) ring_buffer_putc(
            data,
            &rx_buffer );
}


//
static void hw_init( void )
{
    Uart_select( GPS_UART );

    Uart_clear();

    Uart_set_ubrr( GPS_BAUDRATE );

    Uart_hw_init( CONF_8BIT_NOPAR_1STOP );

    gps_uart_enable();
}


//
static uint32_t sbp_read_function(
        uint8_t *buff,
        uint32_t buff_len,
        void *context )
{
    uint32_t idx = 0;

    for( idx = 0; idx < buff_len; idx += 1 )
    {
        const uint16_t data = ring_buffer_getc( &rx_buffer );

        if( data == RING_BUFFER_NO_DATA )
        {
            break;
        }
        else
        {
            buff[ idx ] = (uint8_t) (data & 0xFF);
        }
    }

    return idx;
}


//
static void heartbeat_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    const msg_heartbeat_t * const heartbeat = (const msg_heartbeat_t*) msg;

    DEBUG_PRINTF( "gps_heartbeat : flags 0x%X\n", heartbeat->flags );
}


//
static void gps_time_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_time\n" );

    const uint32_t rx_timestamp = time_get_ms();

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_gps_time_t * const gps_time = (const msg_gps_time_t*) msg;

    gps_state->group_a.time1.rx_time = rx_timestamp;
    gps_state->group_a.time1.time_of_week = gps_time->tow;
    gps_state->group_a.time2.week_number = gps_time->wn;
    gps_state->group_a.time2.residual = gps_time->ns;
    gps_state->group_a.time2.flags = gps_time->flags;

    gps_set_group_ready( GPS_GROUP_A_READY, gps_state );
}


//
static void dops_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_dops\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_dops_t * const dops = (const msg_dops_t*) msg;

    gps_state->group_f.dop1.time_of_week = dops->tow;
    gps_state->group_f.dop1.gdop = dops->gdop;
    gps_state->group_f.dop1.pdop = dops->pdop;
    gps_state->group_f.dop2.tdop = dops->tdop;
    gps_state->group_f.dop2.hdop = dops->hdop;
    gps_state->group_f.dop2.vdop = dops->vdop;

    gps_set_group_ready( GPS_GROUP_F_READY, gps_state );
}


//
static void pos_llh_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_pos_llh\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_pos_llh_t * const pos_llh = (const msg_pos_llh_t*) msg;

    gps_state->group_b.pos_llh1.time_of_week = pos_llh->tow;
    gps_state->group_b.pos_llh1.num_sats = pos_llh->n_sats;
    gps_state->group_b.pos_llh1.flags = pos_llh->flags;
    memcpy(
            &gps_state->group_b.pos_llh2.latitude,
            &pos_llh->lat,
            sizeof(gps_state->group_b.pos_llh2.latitude) );
    memcpy(
            &gps_state->group_b.pos_llh3.longitude,
            &pos_llh->lon,
            sizeof(gps_state->group_b.pos_llh3.longitude) );
    memcpy(
            &gps_state->group_b.pos_llh4.height,
            &pos_llh->height,
            sizeof(gps_state->group_b.pos_llh4.height) );

    gps_set_group_ready( GPS_GROUP_B_READY, gps_state );
}


//
static void baseline_ned_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_baseline_ned\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_baseline_ned_t * const baseline_ned = (const msg_baseline_ned_t*) msg;

    gps_state->group_c.baseline_ned1.time_of_week = baseline_ned->tow;
    gps_state->group_c.baseline_ned1.num_sats = baseline_ned->n_sats;
    gps_state->group_c.baseline_ned1.flags = baseline_ned->flags;
    gps_state->group_c.baseline_ned2.north = baseline_ned->n;
    gps_state->group_c.baseline_ned2.east = baseline_ned->e;
    gps_state->group_c.baseline_ned3.down = baseline_ned->d;

    gps_set_group_ready( GPS_GROUP_C_READY, gps_state );
}


//
static void vel_ned_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_vel_ned\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_vel_ned_t * const vel_ned = (const msg_vel_ned_t*) msg;

    gps_state->group_d.vel_ned1.time_of_week = vel_ned->tow;
    gps_state->group_d.vel_ned1.num_sats = vel_ned->n_sats;
    gps_state->group_d.vel_ned1.flags = vel_ned->flags;
    gps_state->group_d.vel_ned2.north = vel_ned->n;
    gps_state->group_d.vel_ned2.east = vel_ned->e;
    gps_state->group_d.vel_ned3.down = vel_ned->d;

    gps_set_group_ready( GPS_GROUP_D_READY, gps_state );
}


//
static void heading_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_heading\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_baseline_heading_t * const heading = (const msg_baseline_heading_t*) msg;

    gps_state->group_e.heading1.time_of_week = heading->tow;
    gps_state->group_e.heading1.heading = heading->heading;
    gps_state->group_e.heading2.num_sats = heading->n_sats;
    gps_state->group_e.heading2.flags = heading->flags;

    gps_set_group_ready( GPS_GROUP_E_READY, gps_state );
}


//
static uint8_t publish_group_a(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_TIME1,
            (uint8_t) sizeof(gps_state->group_a.time1),
            (const uint8_t *) &gps_state->group_a.time1 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_TIME2,
            (uint8_t) sizeof(gps_state->group_a.time2),
            (const uint8_t *) &gps_state->group_a.time2 );

    return ret;
}


//
static uint8_t publish_group_b(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_POS_LLH1,
            (uint8_t) sizeof(gps_state->group_b.pos_llh1),
            (const uint8_t *) &gps_state->group_b.pos_llh1 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_POS_LLH2,
            (uint8_t) sizeof(gps_state->group_b.pos_llh2),
            (const uint8_t *) &gps_state->group_b.pos_llh2 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_POS_LLH3,
            (uint8_t) sizeof(gps_state->group_b.pos_llh3),
            (const uint8_t *) &gps_state->group_b.pos_llh3 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_POS_LLH4,
            (uint8_t) sizeof(gps_state->group_b.pos_llh4),
            (const uint8_t *) &gps_state->group_b.pos_llh4 );

    return ret;
}


//
static uint8_t publish_group_c(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_BASELINE_NED1,
            (uint8_t) sizeof(gps_state->group_c.baseline_ned1),
            (const uint8_t *) &gps_state->group_c.baseline_ned1 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_BASELINE_NED2,
            (uint8_t) sizeof(gps_state->group_c.baseline_ned2),
            (const uint8_t *) &gps_state->group_c.baseline_ned2 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_BASELINE_NED3,
            (uint8_t) sizeof(gps_state->group_c.baseline_ned3),
            (const uint8_t *) &gps_state->group_c.baseline_ned3 );

    return ret;
}


//
static uint8_t publish_group_d(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_VEL_NED1,
            (uint8_t) sizeof(gps_state->group_d.vel_ned1),
            (const uint8_t *) &gps_state->group_d.vel_ned1 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_VEL_NED2,
            (uint8_t) sizeof(gps_state->group_d.vel_ned2),
            (const uint8_t *) &gps_state->group_d.vel_ned2 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_VEL_NED3,
            (uint8_t) sizeof(gps_state->group_d.vel_ned3),
            (const uint8_t *) &gps_state->group_d.vel_ned3 );

    return ret;
}


//
static uint8_t publish_group_e(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_HEADING1,
            (uint8_t) sizeof(gps_state->group_e.heading1),
            (const uint8_t *) &gps_state->group_e.heading1 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_HEADING2,
            (uint8_t) sizeof(gps_state->group_e.heading2),
            (const uint8_t *) &gps_state->group_e.heading2 );

    return ret;
}


//
static uint8_t publish_group_f(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_DOP1,
            (uint8_t) sizeof(gps_state->group_f.dop1),
            (const uint8_t *) &gps_state->group_f.dop1 );

    ret |= canbus_send(
            HOBD_CAN_ID_GPS_DOP2,
            (uint8_t) sizeof(gps_state->group_f.dop2),
            (const uint8_t *) &gps_state->group_f.dop2 );

    return ret;
}




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t gps_init(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    sbp_state_init( &sbp_state );

#warning "TODO - check SBP cb status"

    int8_t sbp_status = 0;
    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_HEARTBEAT,
            &heartbeat_callback,
            (void*) gps_state,
            &heartbeat_callback_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_GPS_TIME,
            &gps_time_callback,
            (void*) gps_state,
            &gps_time_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_DOPS,
            &dops_callback,
            (void*) gps_state,
            &dops_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_POS_LLH,
            &pos_llh_callback,
            (void*) gps_state,
            &pos_llh_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_BASELINE_NED,
            &baseline_ned_callback,
            (void*) gps_state,
            &baseline_ned_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_VEL_NED,
            &vel_ned_callback,
            (void*) gps_state,
            &vel_ned_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_BASELINE_HEADING,
            &heading_callback,
            (void*) gps_state,
            &heading_node );

    //
    hw_init();

    // clear all ready groups
    gps_clear_all_group_ready( gps_state );

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    return ret;
}


//
void gps_disable( void )
{
    // disable UART
    gps_uart_disable();

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );
}


//
void gps_enable( void )
{
    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    // enable UART
    gps_uart_enable();
}


//
void gps_set_group_ready(
        const uint16_t group,
        gps_state_s * const gps_state )
{
    gps_state->ready_groups |= group;
}


//
void gps_clear_group_ready(
        const uint16_t group,
        gps_state_s * const gps_state )
{
    gps_state->ready_groups &= ~group;
}


//
void gps_clear_all_group_ready(
        gps_state_s * const gps_state )
{
    gps_state->ready_groups = GPS_GROUP_NONE_READY;
}


//
uint8_t gps_is_group_ready(
        const uint16_t group,
        const gps_state_s * const gps_state )
{
    return ((gps_state->ready_groups & group) == 0) ? 0 : 1;
}


//
uint8_t gps_update(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

#warning "TODO - check SBP read status and return code handling"

    // process any available data in the rx buffer, callbacks are called from
    // this context
    const int8_t sbp_status = sbp_process(
            &sbp_state,
            &sbp_read_function );

    if(
            (sbp_status != SBP_OK)
            && (sbp_status != SBP_OK_CALLBACK_EXECUTED)
            && (sbp_status != SBP_OK_CALLBACK_UNDEFINED) )
    {
        DEBUG_PRINTF( "gps_enable : sbp_process %d\n", sbp_status );
    }

    // check for any ready groups
    if( gps_state->ready_groups != GPS_GROUP_NONE_READY )
    {
        // handle groups in order/priority
        if( gps_is_group_ready( GPS_GROUP_A_READY, gps_state ) != 0 )
        {
            ret = publish_group_a( gps_state );
            gps_clear_group_ready( GPS_GROUP_A_READY, gps_state );
        }

        if( gps_is_group_ready( GPS_GROUP_B_READY, gps_state ) != 0 )
        {
            ret = publish_group_b( gps_state );
            gps_clear_group_ready( GPS_GROUP_B_READY, gps_state );
        }

        if( gps_is_group_ready( GPS_GROUP_C_READY, gps_state ) != 0 )
        {
            ret = publish_group_c( gps_state );
            gps_clear_group_ready( GPS_GROUP_C_READY, gps_state );
        }

        if( gps_is_group_ready( GPS_GROUP_D_READY, gps_state ) != 0 )
        {
            ret = publish_group_d( gps_state );
            gps_clear_group_ready( GPS_GROUP_D_READY, gps_state );
        }

        if( gps_is_group_ready( GPS_GROUP_E_READY, gps_state ) != 0 )
        {
            ret = publish_group_e( gps_state );
            gps_clear_group_ready( GPS_GROUP_E_READY, gps_state );
        }

        if( gps_is_group_ready( GPS_GROUP_F_READY, gps_state ) != 0 )
        {
            ret = publish_group_f( gps_state );
            gps_clear_group_ready( GPS_GROUP_F_READY, gps_state );
        }
    }

    return ret;
}
