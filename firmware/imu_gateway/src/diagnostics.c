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
static const uint16_t CAN_ID_HEARTBEAT = (HOBD_CAN_ID_HEARTBEAT_BASE + NODE_ID);




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************




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

#warning "TODO - call init state - and publish on state change"
}


//
void diagnostics_set_state(
        const uint8_t state )
{
    hobd_heartbeat.state = state;
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
void diagnostics_update( void )
{

}
