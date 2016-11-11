/**
 * @file can_replay.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "polysync_core.h"
#include "polysync_message.h"
#include "polysync_logfile.h"
#include "polysync_can.h"
#include "time_domain.h"
#include "can_frame.h"
#include "can.h"




// *****************************************************
// static global types/macros
// *****************************************************




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
can_handle_s can_replay_open(
        const char * const file )
{
    can_handle_s handle = CAN_HANDLE_INVALID;


    return handle;
}


//
void can_replay_close(
        const can_handle_s handle )
{

}


//
int can_replay_read(
        const can_handle_s handle,
        const timestamp_ms timeout,
        can_frame_s * const frame )
{
    int ret = 1;

    return ret;
}
