/**
 * @file gps.h
 * @brief TODO.
 *
 */




#ifndef GPS_H
#define	GPS_H




#include <inttypes.h>




//
uint8_t gps_init( void );


//
void gps_disable( void );


//
void gps_enable( void );


//
uint8_t gps_update( void );




#endif	/* GPS_H */
