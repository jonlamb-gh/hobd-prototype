/**
 * @file gps.h
 * @brief TODO.
 *
 */




#ifndef GPS_H
#define	GPS_H




#include <inttypes.h>

#include "hobd.h"




// TODO - grouping/flags for CAN publishing signals
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
    hobd_gps_pos_ecef1_s pos_ecef1;
    //
    //
    hobd_gps_pos_ecef2_s pos_ecef2;
    //
    //
    hobd_gps_pos_ecef3_s pos_ecef3;
    //
    //
    hobd_gps_pos_ecef4_s pos_ecef4;
    //
    //
    hobd_gps_baseline_ecef1_s baseline_ecef1;
    //
    //
    hobd_gps_baseline_ecef2_s baseline_ecef2;
    //
    //
    hobd_gps_baseline_ecef3_s baseline_ecef3;
    //
    //
    hobd_gps_vel_ecef1_s vel_ecef1;
    //
    //
    hobd_gps_vel_ecef2_s vel_ecef2;
    //
    //
    hobd_gps_vel_ecef3_s vel_ecef3;
    //
    //
    hobd_gps_heading1_s heading1;
    //
    //
    hobd_gps_heading2_s heading2;
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
