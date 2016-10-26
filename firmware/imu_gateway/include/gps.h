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
    hobd_gps_time1_s time1;
    //
    //
    hobd_gps_time2_s time2;
    //
    //
    hobd_gps_dop1_s dop1;
    //
    //
    hobd_gps_dop2_s dop2;
    //
    //
    hobd_gps_pos_ecef1_s pos_edef1;
    //
    //
    hobd_gps_pos_ecef2_s pos_edef2;
    //
    //
    hobd_gps_pos_ecef3_s pos_edef3;
    //
    //
    hobd_gps_pos_ecef4_s pos_edef4;
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
