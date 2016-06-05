/**
 * @file can_communication.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "can_lib.h"
#include "hobd.h"
#include "hobd_can.h"
#include "can_frame.h"
#include "can_communication.h"




// *****************************************************
// static global types/macros
// *****************************************************
#warning "need to handle blocking/waiting/error in senc/recv"




// *****************************************************
// static global data
// *****************************************************




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
uint8_t cancomm_init( void )
{
    uint8_t ret = 0;

    if( can_init( 0 ) == 0 )
    {
        ret = 1;
    }

    return ret;
}


//
uint8_t cancomm_send( const can_frame * const frame )
{
    uint8_t ret = 0;
    uint8_t status = 0;
    st_cmd_t cmd;

    // construct canlib command
    cmd.id.std = (uint16_t) frame->id;
    cmd.dlc = (uint8_t) frame->dlc;
    cmd.pt_data = (uint8_t*) frame->data;
    cmd.status = 0;
    cmd.ctrl.rtr = 0;
    cmd.ctrl.ide = 0;

    // command type - send data
    cmd.cmd = CMD_TX_DATA;

    // send the data to an available MOB
    do
    {
        status = can_cmd( &cmd );
    }
    while( status != CAN_CMD_ACCEPTED );

    // wait for completion
    do
    {
        status = can_get_status( &cmd );

        if( status == CAN_STATUS_ERROR )
        {
            // set error
            ret = 1;

            // force return now
            status = CAN_STATUS_COMPLETED;
        }
    }
    while( status != CAN_STATUS_COMPLETED );

    return ret;
}


//
uint8_t cancomm_recv( can_frame * const frame )
{
    uint8_t ret = 0;
//    uint8_t status = 0;
//    st_cmd_t cmd;
//
//    // construct canlib command
//    cmd.id.std = (uint16_t) frame->id;
//    cmd.dlc = 0;
//    cmd.pt_data = (uint8_t*) frame->data;
//    cmd.status = 0;
//    cmd.ctrl.rtr = 0;
//    cmd.ctrl.ide = 0;
//
//    // command type - receive data
//    cmd.cmd = CMD_RX_DATA;

    // send the data to an available MOB
//    do
//    {
//        status = can_cmd( &cmd );
//    }
//    while( status != CAN_CMD_ACCEPTED );
//
//    // check for completion
//    status = can_get_status( &cmd );
//
//    if( status == CAN_STATUS_ERROR )
//    {
//        // set error
//        ret = 1;
//    }
//    else if( status == CAN_STATUS_COMPLETED )
//    {
//        // data received
//        ret = 2;
//
//        frame->dlc = (uint8_t) cmd.dlc;
//    }
#warning "cancomm_recv disabled"
    ret = 0;

    return ret;
}
