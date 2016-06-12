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
#define CANCOMM_STATE_ID_INVALID (0)


//
#define CANCOMM_STATE_ID_MAX (15)


//
#define CANCOMM_RX_TIMEOUT (4)




//
uint8_t cancomm_init( void );


//
uint8_t cancomm_send( const can_frame * const frame );


//
uint8_t cancomm_recv( can_frame * const frame );




#endif	/* CAN_COMMUNICATION_H */
