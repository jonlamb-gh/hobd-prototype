/**
 * @file gps.h
 * @brief TODO.
 *
 */




#ifndef GPS_H
#define	GPS_H




#include <inttypes.h>

#include "hobd.h"




//
typedef struct
{
    //
    //
    hobd_gps_time1_s gps_time1;
    //
    //
    hobd_gps_time2_s gps_time2;
} gps_state_s;




//
uint8_t gps_init(
        gps_state_s * const gps_state );


//
void gps_disable( void );


//
void gps_enable( void );


//
uint8_t gps_update(
        gps_state_s * const gps_state );




#endif	/* GPS_H */
