/**
 * @file gps.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
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
#include "gps.h"




// *****************************************************
// static global types/macros
// *****************************************************

// GPS is on UART1
#define UART_RX_INTERRUPT USART1_RX_vect
#define UART_STATUS UCSR1A
#define UART_CONTROL UCSR1B
#define UART_DATA UDR1


//
#define gps_uart_enable() (UART_CONTROL |= (_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))


//
#define gps_uart_disable() (UART_CONTROL &= ~(_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))




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
static sbp_msg_callbacks_node_t pos_ecef_node;
static sbp_msg_callbacks_node_t baseline_ecef_node;
static sbp_msg_callbacks_node_t vel_ecef_node;
static sbp_msg_callbacks_node_t heading_node;




// *****************************************************
// static declarations
// *****************************************************

//
static uint8_t hw_init( void );


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
static void pos_ecef_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void baseline_ecef_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context );


//
static void vel_ecef_callback(
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




// *****************************************************
// static definitions
// *****************************************************

//
ISR( UART_RX_INTERRUPT )
{
    // read UART status register and UART data register
    const uint8_t status  = UART_STATUS;
    const uint8_t data = UART_DATA;

    // read error status
    rx_buffer.error = (status & (_BV(FE1) | _BV(DOR1)) );

    // push data into the rx buffer, error is updated with return status
    (void) ring_buffer_putc(
            data,
            &rx_buffer );
}


//
static uint8_t hw_init( void )
{
    uint8_t ret = 0;

    Uart_select( GPS_UART );

    Uart_clear();

    if( Uart_set_baudrate( GPS_BAUDRATE ) == 0 )
    {
        ret = 1;
    }
    else
    {
        Uart_hw_init( CONF_8BIT_NOPAR_1STOP );
        gps_uart_enable();
    }

    return ret;
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

    DEBUG_PRINTF( "gps_heartbeat : flags 0x%X\r\n", heartbeat->flags );
}


//
static void gps_time_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_time\r\n" );

    const uint32_t rx_timestamp = time_get_ms();

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_gps_time_t * const gps_time = (const msg_gps_time_t*) msg;

    gps_state->time1.rx_time = rx_timestamp;
    gps_state->time1.time_of_week = gps_time->tow;
    gps_state->time2.week_number = gps_time->wn;
    gps_state->time2.residual = gps_time->ns;
    gps_state->time2.reserved = (uint16_t) gps_time->flags;
}


//
static void dops_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_dops\r\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_dops_t * const dops = (const msg_dops_t*) msg;

    gps_state->dop1.time_of_week = dops->tow;
    gps_state->dop1.gdop = dops->gdop;
    gps_state->dop1.pdop = dops->pdop;
    gps_state->dop2.tdop = dops->tdop;
    gps_state->dop2.hdop = dops->hdop;
    gps_state->dop2.vdop = dops->vdop;
}


//
static void pos_ecef_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_pos_ecef\r\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_pos_ecef_t * const pos_ecef = (const msg_pos_ecef_t*) msg;

    gps_state->pos_ecef1.time_of_week = pos_ecef->tow;
    gps_state->pos_ecef1.accuracy = pos_ecef->accuracy;
    gps_state->pos_ecef1.num_sats = pos_ecef->n_sats;
    gps_state->pos_ecef1.flags = pos_ecef->flags;
    memcpy(
            &gps_state->pos_ecef2.x,
            &pos_ecef->x,
            sizeof(gps_state->pos_ecef2.x) );
    memcpy(
            &gps_state->pos_ecef3.y,
            &pos_ecef->y,
            sizeof(gps_state->pos_ecef3.y) );
    memcpy(
            &gps_state->pos_ecef4.z,
            &pos_ecef->z,
            sizeof(gps_state->pos_ecef4.z) );
}


//
static void baseline_ecef_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_baseline_ecef\r\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_baseline_ecef_t * const baseline_ecef = (const msg_baseline_ecef_t*) msg;

    gps_state->baseline_ecef1.time_of_week = baseline_ecef->tow;
    gps_state->baseline_ecef1.accuracy = baseline_ecef->accuracy;
    gps_state->baseline_ecef1.num_sats = baseline_ecef->n_sats;
    gps_state->baseline_ecef1.flags = baseline_ecef->flags;
    gps_state->baseline_ecef2.x = baseline_ecef->x;
    gps_state->baseline_ecef2.y = baseline_ecef->y;
    gps_state->baseline_ecef3.z = baseline_ecef->z;
}


//
static void vel_ecef_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    DEBUG_PUTS( "gps_vel_ecef\r\n" );

    gps_state_s * const gps_state = (gps_state_s*) context;
    const msg_vel_ecef_t * const vel_ecef = (const msg_vel_ecef_t*) msg;

    gps_state->vel_ecef1.time_of_week = vel_ecef->tow;
    gps_state->vel_ecef1.accuracy = vel_ecef->accuracy;
    gps_state->vel_ecef1.num_sats = vel_ecef->n_sats;
    gps_state->vel_ecef1.flags = vel_ecef->flags;
    gps_state->vel_ecef2.x = vel_ecef->x;
    gps_state->vel_ecef2.y = vel_ecef->y;
    gps_state->vel_ecef3.z = vel_ecef->z;
}


//
static void heading_callback(
        uint16_t sender_id,
        uint8_t msg_len,
        uint8_t msg[],
        void *context )
{
    //const msg_baseline_heading_t * const heading = (const msg_baseline_heading_t*) msg;

    DEBUG_PUTS( "gps_heading\r\n" );
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
            SBP_MSG_POS_ECEF,
            &pos_ecef_callback,
            (void*) gps_state,
            &pos_ecef_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_BASELINE_ECEF,
            &baseline_ecef_callback,
            (void*) gps_state,
            &baseline_ecef_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_VEL_ECEF,
            &vel_ecef_callback,
            (void*) gps_state,
            &vel_ecef_node );

    sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_BASELINE_HEADING,
            &heading_callback,
            (void*) gps_state,
            &heading_node );

    ret = hw_init();

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
uint8_t gps_update(
        gps_state_s * const gps_state )
{
    uint8_t ret = 0;

#warning "TODO - check SBP read status"

    // process any available data in the rx buffer, callbacks are called from
    // this context
    const int8_t sbp_status = sbp_process(
            &sbp_state,
            &sbp_read_function );

    if( sbp_status != SBP_OK )
    {
        DEBUG_PRINTF( "gps_enable : sbp_process %d\r\n", sbp_status );
    }

    return ret;
}
