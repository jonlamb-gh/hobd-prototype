/**
 * @file obd.h
 * @brief TODO.
 *
 */




#ifndef OBD_H
#define	OBD_H




#include <inttypes.h>

#include "hobd.h"




// ms
//#define GPS_FIX_WARN_TIMEOUT (5000UL)




// GPS message data group
typedef struct
{
    //
    //
    uint16_t ready_groups;
    //
    // GPS_GROUP_A_READY
    struct
    {
        //
        //
        hobd_gps_time1_s time1;
        //
        //
        hobd_gps_time2_s time2;
    } group_a;
    //
    // GPS_GROUP_B_READY
    struct
    {
        //
        //
        hobd_gps_pos_llh1_s pos_llh1;
        //
        //
        hobd_gps_pos_llh2_s pos_llh2;
        //
        //
        hobd_gps_pos_llh3_s pos_llh3;
        //
        //
        hobd_gps_pos_llh4_s pos_llh4;
    } group_b;
    //
    // GPS_GROUP_C_READY
    struct
    {
        //
        //
        hobd_gps_baseline_ned1_s baseline_ned1;
        //
        //
        hobd_gps_baseline_ned2_s baseline_ned2;
        //
        //
        hobd_gps_baseline_ned3_s baseline_ned3;
    } group_c;
    //
    // GPS_GROUP_D_READY
    struct
    {
        //
        //
        hobd_gps_vel_ned1_s vel_ned1;
        //
        //
        hobd_gps_vel_ned2_s vel_ned2;
        //
        //
        hobd_gps_vel_ned3_s vel_ned3;
    } group_d;
    //
    // GPS_GROUP_E_READY
    struct
    {
        //
        //
        hobd_gps_heading1_s heading1;
        //
        //
        hobd_gps_heading2_s heading2;
    } group_e;
    //
    // GPS_GROUP_F_READY
    struct
    {
        //
        //
        hobd_gps_dop1_s dop1;
        //
        //
        hobd_gps_dop2_s dop2;
    } group_f;
} gps_data_s;




//
uint8_t gps_init( void );


//
void gps_disable( void );


//
void gps_enable( void );


//
void gps_set_group_ready(
        const uint16_t group );


//
void gps_clear_group_ready(
        const uint16_t group );


//
void gps_clear_all_group_ready( void );


//
uint8_t gps_is_group_ready(
        const uint16_t group );


//
uint8_t gps_update( void );




#endif	/* OBD_H */
