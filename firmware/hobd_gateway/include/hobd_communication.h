/**
 * @file hobd_communication.h
 * @brief TODO.
 *
 */




#ifndef HOBD_COMMUNICATION_H
#define	HOBD_COMMUNICATION_H




#include <stdint.h>
#include "hobd.h"




//
uint8_t hobdcomm_init( void );


//
uint8_t hobdcomm_send( void );


//
uint8_t hobdcomm_recv(
        uint8_t * const buffer,
        const uint8_t max_len );


//
uint8_t hobdcomm_checksum( const uint8_t * const buffer, const uint8_t len );


//
uint8_t hobdcomm_is_valid_packet( const uint8_t * const buffer, const uint8_t len );



#endif	/* HOBD_COMMUNICATION_H */
