/**
 * @file diagnostics.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <inttypes.h>

#include "board.h"
#include "hobd.h"
#include "time.h"
#include "canbus.h"
#include "diagnostics.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************

//
static hobd_heartbeat_s hobd_heartbeat;


//
static uint32_t last_tx_heartbeat = 0;


//
static uint32_t last_warn_clear = 0;


//
static uint32_t warn_clear_set = DIAGNOSTICS_CLEAR_SET_NONE;


//
static const uint16_t CAN_ID_HEARTBEAT =
        (uint16_t) (HOBD_CAN_ID_HEARTBEAT_BASE + NODE_ID);




// *****************************************************
// static declarations
// *****************************************************


//
static void send_heartbeat(
        const uint32_t * const now,
        const uint8_t send_now );


//
static void clear_warn_set(
        const uint32_t * const now,
        const uint8_t clear_now );




// *****************************************************
// static definitions
// *****************************************************

//
static void send_heartbeat(
        const uint32_t * const now,
        const uint8_t send_now )
{
    // get time since last publish
    const uint32_t delta = time_get_delta(
            &last_tx_heartbeat,
            now );

    // publish if interval met/exceeded
    if( (send_now != 0) || (delta >= HOBD_CAN_TX_INTERVAL_HEARTBEAT) )
    {
        // update last publish timestamp, ms
        last_tx_heartbeat = (*now);

        // update counter
        hobd_heartbeat.counter += 1;

        // publish
        const uint8_t ret = canbus_send(
                CAN_ID_HEARTBEAT,
                (uint8_t) sizeof(hobd_heartbeat),
                (const uint8_t*) &hobd_heartbeat );
        if( ret != 0 )
        {
            diagnostics_set_warn( HOBD_HEARTBEAT_WARN_CANBUS );
        }
    }
}


//
static void clear_warn_set(
        const uint32_t * const now,
        const uint8_t clear_now )
{
    // if any warning bits are to be cleared
    if( warn_clear_set != DIAGNOSTICS_CLEAR_SET_NONE )
    {
        // get time since last clear
        const uint32_t delta = time_get_delta(
                &last_warn_clear,
                now );

        // clear if interval met/exceeded
        if( (clear_now != 0) || (delta >= DIAGNOSTICS_WARN_SET_CLEAR_INTERVAL) )
        {
            // update last clear timestamp, ms
            last_warn_clear = (*now);

            // clear warning bits
            diagnostics_clear_warn( warn_clear_set );
        }
    }
}




// *****************************************************
// public definitions
// *****************************************************

//
void diagnostics_init( void )
{
    hobd_heartbeat.hardware_version = HARDWARE_VERSION;
    hobd_heartbeat.firmware_version = FIRMWARE_VERSION;
    hobd_heartbeat.node_id = NODE_ID;
    hobd_heartbeat.state = HOBD_HEARTBEAT_STATE_INIT;
    hobd_heartbeat.counter = 0;
    hobd_heartbeat.error_register = 0;
    hobd_heartbeat.warning_register = 0;
}


//
void diagnostics_set_state(
        const uint8_t state )
{
    if( state != hobd_heartbeat.state )
    {
        // get current time
        const uint32_t now = time_get_ms();

        hobd_heartbeat.state = state;

        // publish heartbeat message immediately
        send_heartbeat( &now, 1 );
    }
}


//
uint8_t diagnostics_get_state( void )
{
    return hobd_heartbeat.state;
}


//
void diagnostics_set_warn(
        const uint16_t warn )
{
    hobd_heartbeat.warning_register |= warn;
}


//
uint16_t diagnostics_get_warn( void )
{
    return hobd_heartbeat.warning_register;
}


//
void diagnostics_clear_warn(
        const uint16_t warn )
{
    hobd_heartbeat.warning_register &= ~warn;
}


//
void diagnostics_set_error(
        const uint16_t error )
{
    hobd_heartbeat.error_register |= error;
}


//
uint16_t diagnostics_get_error( void )
{
    return hobd_heartbeat.error_register;
}


//
void diagnostics_clear_error(
        const uint16_t error )
{
    hobd_heartbeat.error_register &= ~error;
}


//
void diagnostics_set_warn_timeout_bits(
        const uint16_t bits )
{
    warn_clear_set = bits;
}


//
void diagnostics_update( void )
{
    // get current time
    const uint32_t now = time_get_ms();

    // transition to an error state if any error bits are set
    if( hobd_heartbeat.error_register != 0 )
    {
        diagnostics_set_state( HOBD_HEARTBEAT_STATE_ERROR );
    }
    else
    {
        // recover from errors
        if( hobd_heartbeat.state == HOBD_HEARTBEAT_STATE_ERROR )
        {
            diagnostics_set_state( HOBD_HEARTBEAT_STATE_OK );
        }
    }

    // clear warning bits as needed
    clear_warn_set( &now, 0 );

    // publish heartbeat message as needed
    send_heartbeat( &now, 0 );
}
