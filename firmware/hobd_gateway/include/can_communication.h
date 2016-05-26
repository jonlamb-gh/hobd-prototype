/**
 * @file can_communication.h
 * @brief TODO.
 *
 */




#ifndef CAN_COMMUNICATION_H
#define	CAN_COMMUNICATION_H




#include <stdint.h>
#include "can_frame.h"
#include "hobd_can.h"




//
uint8_t cancomm_init( void );


//
uint8_t cancomm_send( const can_frame_s * const frame );


//
uint8_t cancomm_recv( can_frame_s * const frame );




#endif	/* CAN_COMMUNICATION_H */
