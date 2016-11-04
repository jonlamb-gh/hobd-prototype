/**
 * @file can.h
 * @brief TODO.
 *
 */




#ifndef CAN_H
#define CAN_H




#include "time_domain.h"




//
typedef int can_handle;




//
typedef struct
{
    //
    //
    unsigned long id;
    //
    //
    unsigned long dlc;
    //
    //
    timestamp_ms native_rx_timestamp;
    //
    //
    timestamp_ms rx_timestamp;
    //
    //
    timestamp_ms rx_timestamp_mono;
    //
    //
    unsigned char data[8];
} can_frame_s;




//
int can_open(
        const unsigned long system_id );


//
void can_close(
        const can_handle handle );


//
int can_read(
        const can_handle handle,
        const timestamp_ms timeout,
        can_frame_s * const frame );




#endif /* CAN_H */
