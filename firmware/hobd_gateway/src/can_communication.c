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
#include "time.h"




// *****************************************************
// static global types/macros
// *****************************************************

//
#define STATE_ID_TO_INDEX(x) ((x - 1))




// *****************************************************
// static global data
// *****************************************************

//
static st_cmd_t global_state_array[CANCOMM_STATE_ID_MAX];




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

    memset( global_state_array, 0, sizeof(global_state_array) );

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
    st_cmd_t *cmd = NULL;
    st_cmd_t default_cmd;

    // use a local default handle if not using one of the globals
    if( (frame->state_id == CANCOMM_STATE_ID_INVALID) || (frame->state_id >= CANCOMM_STATE_ID_MAX) )
    {
        // using local
        cmd = &default_cmd;

        // zero
        cmd->status = 0;
        cmd->ctrl.rtr = 0;
        cmd->ctrl.ide = 0;
    }
    else
    {
        // using global
        cmd = &global_state_array[ frame->state_id ];
    }

    // construct canlib command
    cmd->id.std = (uint16_t) frame->id;
    cmd->dlc = (uint8_t) frame->dlc;
    cmd->pt_data = (uint8_t*) frame->data;

    // command type - send data
    cmd->cmd = CMD_TX_DATA;

    // send the data to an available MOB
    do
    {
        status = can_cmd( cmd );
    }
    while( status != CAN_CMD_ACCEPTED );

    // wait for completion
    do
    {
        status = can_get_status( cmd );

        if( status == CAN_STATUS_ERROR )
        {
            // set error
            ret = 1;

            // force return now
            status = CAN_STATUS_COMPLETED;
        }
    }
    while( status != CAN_STATUS_COMPLETED );

    // clear status
    cmd->status = 0;
    cmd->cmd = 0;

    return ret;
}


//
uint8_t cancomm_recv( can_frame * const frame )
{
    uint8_t ret = 0;
    uint8_t status = CAN_CMD_REFUSED;
    uint32_t delta_time = 0;
    uint8_t do_loop = 0;
    st_cmd_t *cmd = NULL;
    st_cmd_t default_cmd;

    // use a local default handle if not using one of the globals
    if( (frame->state_id == CANCOMM_STATE_ID_INVALID) || (frame->state_id >= CANCOMM_STATE_ID_MAX) )
    {
        // using local
        cmd = &default_cmd;

        // zero
        cmd->status = 0;
        cmd->ctrl.rtr = 0;
        cmd->ctrl.ide = 0;
    }
    else
    {
        // using global
        cmd = &global_state_array[ frame->state_id ];
    }

    // construct canlib command
    cmd->id.std = (uint16_t) frame->id;
    cmd->dlc = 0;
    cmd->pt_data = (uint8_t*) frame->data;

    // command type - receive data
    cmd->cmd = CMD_RX_DATA;

    // enable loop, disabled by timeout/error
    do_loop = 1;

    // get current time for timeout
    const uint32_t start_time = time_get_ms();

    // send the data to an available MOB
    do
    {
        status = can_cmd( cmd );

        delta_time = time_get_delta( start_time, time_get_ms() );
        if( delta_time >= CANCOMM_RX_TIMEOUT )
        {
            // timeout
            do_loop = 0;

            // set timeout
            ret = 3;

            // abort
            cmd->cmd = CMD_ABORT;
            (void) can_cmd( cmd );

            // clear status
            cmd->status = 0;
            cmd->cmd = 0;
        }

        if( status == CAN_CMD_ACCEPTED )
        {
            do_loop = 0;
        }
    }
    while( do_loop != 0 );

    // enable the next loop if we succeeded
    if( status == CAN_CMD_ACCEPTED )
    {
        do_loop = 1;
    }

    // check for completion
    while( do_loop != 0 )
    {
        status = can_get_status( cmd );

        delta_time = time_get_delta( start_time, time_get_ms() );

        if( status == CAN_STATUS_ERROR )
        {
            // set error
            ret = 1;

            do_loop = 0;

            // abort
            cmd->cmd = CMD_ABORT;
            (void) can_cmd( cmd );

            // clear status
            cmd->status = 0;
            cmd->cmd = 0;
        }
        else if( status == CAN_STATUS_COMPLETED )
        {
            // data received
            ret = 2;

            do_loop = 0;

            // clear status
            cmd->status = 0;
            cmd->cmd = 0;

            // copy dlc
            frame->dlc = (uint8_t) cmd->dlc;

            // copy id
            frame->id = (uint32_t) cmd->id.std;
        }
        else if( delta_time >= CANCOMM_RX_TIMEOUT )
        {
            // timeout
            do_loop = 0;

            // set timeout
            ret = 3;

            // abort
            cmd->cmd = CMD_ABORT;
            (void) can_cmd( cmd );

            // clear status
            cmd->status = 0;
            cmd->cmd = 0;
        }
    }

    return ret;
}
