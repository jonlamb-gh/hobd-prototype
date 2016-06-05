/**
 * @file main.c
 * @brief Main.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

//
#include "board.h"

// BSP utilities
#include "timer8_drv.h"
#include "timer16_drv.h"
#include "uart_drv.h"
#include "uart_lib.h"
#include "rtc_drv.h"
#include "can_drv.h"
#include "can_lib.h"

//
#include "hobd.h"
#include "hobd_can.h"
#include "time.h"
#include "diagnostic_table.h"
#include "can_communication.h"
#include "hobd_communication.h"

#ifdef BUILD_TYPE_DEBUG
#warning "BUILD_TYPE_DEBUG"
#endif




// *****************************************************
// static global types/macros
// *****************************************************

//
#define NODE_ID (0x05)


//
#define HARDWARE_VERSION (1)


//
#define FIRMWARE_VERSION (1)


//
#define NUM_ECU_DTABLES (3)


//
#define I_TABLE_0 (0)
#define I_TABLE_16 (1)
#define I_TABLE_32 (2)


//
#define SET_WARNING(x) (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->warning_register |= ((uint16_t) x))


//
#define CLEAR_WARNING(x) (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->warning_register &= ~((uint16_t) x))


//
#define SET_ERROR(x) (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->error_register |= ((uint16_t) x))


//
#define CLEAR_ERROR(x) (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->error_register &= ~((uint16_t) x))


//
#define SET_STATE(x) (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->state = ((uint8_t) x))


//
#define GET_STATE() (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->state)


//
#define UPDATE_HEARTBEAT_COUNTER() (((hobd_can_heartbeat_msg*) &can_tx_heartbeat_frame.data)->counter += 1)




// *****************************************************
// static global data
// *****************************************************

//
static diagnostic_table ecu_tables[NUM_ECU_DTABLES];


//
static uint8_t hobd_packet_buffer[HOBD_PACKET_SIZE_MAX];


//
static can_frame can_tx_heartbeat_frame;


//
static can_frame can_tx_response_frame;


//
static can_frame can_tx_dtable_frame;


//
static can_frame can_rx_command_frame;




// *****************************************************
// static declarations
// *****************************************************

//
static void init( void );


//
static void zero_can_tx_frames( void );


//
static void zero_can_rx_frames( void );


//
static void send_can_heartbeat( void );


//
static void update_hobd_communications( void );


//
static void process_can_command( void );




// *****************************************************
// static definitions
// *****************************************************

//
static void init( void )
{
    // disable watchdog
    wdt_disable();

    // init on-board switch pin
    sw0_init();
    sw0_enable_pullup();

    // init enclosure switch pin
    sw1_init();
    sw1_enable_pullup();

    // init LED pin
    led_init();
    led_off();

    // enable watchdog, reset after 120 ms
    wdt_enable( WDTO_120MS );

    // reset watchdog
    wdt_reset();

    // zero globals
    memset( ecu_tables, 0, sizeof(ecu_tables) );
    memset( hobd_packet_buffer, 0, sizeof(hobd_packet_buffer) );
    zero_can_tx_frames();
    zero_can_rx_frames();

    //
    SET_STATE( HOBD_CAN_HEARTBEAT_STATE_INIT );

    //
    rtc_int_init();

    //
    if( cancomm_init() != 0 )
    {
        SET_WARNING( HOBD_CAN_HEARTBEAT_WARN_CANBUS );
    }

    //
    if( hobdcomm_init() != 0 )
    {
        SET_WARNING( HOBD_CAN_HEARTBEAT_WARN_HOBDBUS );
    }

    //
    if( cancomm_send( &can_tx_heartbeat_frame ) != 0 )
    {
        SET_WARNING( HOBD_CAN_HEARTBEAT_WARN_CANBUS );
    }

    //
    enable_interrupt();

    //
#ifdef BUILD_TYPE_DEBUG
	Uart_select( DEBUG_UART );
    uart_init( CONF_8BIT_NOPAR_1STOP, DEBUG_BAUDDRATE );
    Uart_select( HOBD_UART );
#endif

    DEBUG_PUTS( "init : pass\r\n" );
}


//
static void zero_can_tx_frames( void )
{
    //
    can_tx_heartbeat_frame.id = (uint32_t) (HOBD_CAN_ID_HEARTBEAT_BASE + NODE_ID);
    can_tx_heartbeat_frame.dlc = (uint8_t) sizeof(hobd_can_heartbeat_msg);
    can_tx_heartbeat_frame.timestamp = 0;
    memset( (void*) can_tx_heartbeat_frame.data, 0, sizeof(can_tx_heartbeat_frame.data) );

    //
    hobd_can_heartbeat_msg * const heartbeat =
            (hobd_can_heartbeat_msg*) can_tx_heartbeat_frame.data;
    heartbeat->hardware_version = (uint8_t) HARDWARE_VERSION;
    heartbeat->firmware_version = (uint8_t) FIRMWARE_VERSION;
    heartbeat->node_id = (uint8_t) NODE_ID;

    //
    can_tx_response_frame.id = (uint32_t) HOBD_CAN_ID_RESPONSE;
    can_tx_response_frame.dlc = (uint8_t) sizeof(hobd_can_response_msg);
    can_tx_response_frame.timestamp = 0;
    memset( (void*) can_tx_response_frame.data, 0, sizeof(can_tx_response_frame.data) );

    //
    can_tx_dtable_frame.id = (uint32_t) HOBD_CAN_ID_DTABLE_REGISTER;
    can_tx_dtable_frame.dlc = (uint8_t) sizeof(hobd_can_dtable_register_msg);
    can_tx_dtable_frame.timestamp = 0;
    memset( (void*) can_tx_dtable_frame.data, 0, sizeof(can_tx_dtable_frame.data) );
}


//
static void zero_can_rx_frames( void )
{
    //
    can_rx_command_frame.id = (uint32_t) HOBD_CAN_ID_COMMAND;
    can_rx_command_frame.dlc = (uint8_t) sizeof(hobd_can_command_msg);
    can_rx_command_frame.timestamp = 0;
    memset( (void*) can_rx_command_frame.data, 0, sizeof(can_rx_command_frame.data) );
}


//
static void send_can_heartbeat( void )
{
    // time since last publish
    uint32_t delta = 0;
    uint32_t now = 0;

    // get current time
    now = time_get_ms();

    // get time since last publish
    delta = time_get_delta( can_tx_heartbeat_frame.timestamp, now );

    // publish if interval met/exceeded
    if( delta >= HOBD_CAN_TX_INTERVAL_HEARTBEAT )
    {
        // update last publish timestamp, ms
        can_tx_heartbeat_frame.timestamp = now;

        // update counter
        UPDATE_HEARTBEAT_COUNTER();

        // publish
        if( cancomm_send( &can_tx_heartbeat_frame ) != 0 )
        {
            SET_WARNING( HOBD_CAN_HEARTBEAT_WARN_CANBUS );
        }
    }
}


//
static void update_hobd_communications( void )
{
    const uint8_t bytes_read = hobdcomm_recv( hobd_packet_buffer, sizeof(hobd_packet_buffer) );

    const uint8_t packet_type = hobdcomm_is_valid_packet( hobd_packet_buffer, bytes_read );

    //
    if( packet_type == HOBD_PACKET_TYPE_RESPONSE )
    {
        // get rx timestamp - time after data was received
        const uint32_t now = time_get_ms();

        // cast header
        const hobd_packet_header * const header =
                (hobd_packet_header*) hobd_packet_buffer;

        // process
        if( header->subtype == HOBD_PACKET_SUBTYPE_TABLE_SUBGROUP )
        {
            // cast subtable query
            const hobd_table_response * const resp =
                    (hobd_table_response*) hobd_packet_buffer;

            // diagnostic table array index to update
            uint8_t table_index = I_TABLE_0;

            //
            if( resp->table == HOBD_TABLE_0 )
            {
                table_index = I_TABLE_0;
            }
            else if( resp->table == HOBD_TABLE_16 )
            {
                table_index = I_TABLE_16;
            }
            else if( resp->table == HOBD_TABLE_32 )
            {
                table_index = I_TABLE_32;
            }

            //
            dtable_update(
                        hobd_packet_buffer,
                        now,
                        &ecu_tables[ table_index ] );
        }
    }
}


//
static void process_can_command( void )
{
    // time since last rx check
    uint32_t delta = 0;
    uint32_t now = 0;

    // invalidate/clear the global frame
    can_rx_command_frame.id = (uint32_t) HOBD_CAN_ID_COMMAND;
    can_rx_command_frame.dlc = (uint8_t) sizeof(hobd_can_command_msg);
    memset( (void*) can_rx_command_frame.data, 0, sizeof(can_rx_command_frame.data) );

    // get current time
    now = time_get_ms();

    // get time since last rx check
    delta = time_get_delta( can_rx_command_frame.timestamp, now );

    // check for data if interval met/exceeded
    if( delta >= HOBD_CAN_RX_INTERVAL_COMMAND )
    {
        // update last rx check timestamp, ms
        can_rx_command_frame.timestamp = now;

        // check for rx message
        const uint8_t ret = cancomm_recv( &can_rx_command_frame );

        if( ret == 2 )
        {
            // data received

            // TODO
            // handle command
        }
        else if( ret == 1 )
        {
            SET_WARNING( HOBD_CAN_HEARTBEAT_WARN_CANBUS );
        }
    }
}




// *****************************************************
// main
// *****************************************************
int main( void )
{
    //
    init();

    //
    SET_STATE( HOBD_CAN_HEARTBEAT_STATE_OK );

    //
    while( 1 )
    {
        // reset watchdog
        wdt_reset();

#warning "TODO - hobd command/response framework - currently only receiving is enabled"
        // process ready commands and available HOBD messages
        update_hobd_communications();

        // check and process available CAN command messages
        process_can_command();

        // send periodic CAN heartbeat message
        send_can_heartbeat();

        // turn on the on-board LED if either switch is closed
        if( (sw0_get_state() == ON) || (sw1_get_state() == ON) )
        {
            led_on();
        }
        else
        {
            led_off();
        }

#warning "TODO - clear warnings after some time"
        // clear warnings
        CLEAR_WARNING( HOBD_CAN_HEARTBEAT_WARN_CANBUS );
    }

   return 0;
}
