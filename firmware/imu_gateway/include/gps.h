/**
 * @file gps.h
 * @brief TODO.
 *
 */




#ifndef GPS_H
#define	GPS_H




#include <inttypes.h>

#include "hobd.h"




// available GPS position coordinates
#define GPS_POSITION_LLH (0)
#define GPS_POSITION_EFEC (1)


//
#define GPS_VELOCITY_NED (0)
#define GPS_VELOCITY_ECEF (1)


//
#define GPS_BASELINE_NED (0)
#define GPS_BASELINE_ECEF (1)


//
#define GPS_GROUP_NONE_READY (0x0000)
#define GPS_GROUP_A_READY (1 << 0)
#define GPS_GROUP_B_READY (1 << 1)
#define GPS_GROUP_C_READY (1 << 2)
#define GPS_GROUP_D_READY (1 << 3)
#define GPS_GROUP_E_READY (1 << 4)
#define GPS_GROUP_F_READY (1 << 5)




// TODO - should this be in hobd.h as a global definition ?
typedef struct
{
    //
    //
    uint16_t ready_groups;
    //
    // TODO - flag bit MACRO for each group in docs here
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
    //
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
    //
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
    //
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
    //
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
    //
    struct
    {
        //
        //
        hobd_gps_dop1_s dop1;
        //
        //
        hobd_gps_dop2_s dop2;
    } group_f;
} gps_state_s;




//
uint8_t gps_init(
        gps_state_s * const gps_state );


//
void gps_disable( void );


//
void gps_enable( void );


//
void gps_set_group_ready(
        const uint16_t group,
        gps_state_s * const gps_state );


//
void gps_clear_group_ready(
        const uint16_t group,
        gps_state_s * const gps_state );


//
void gps_clear_all_group_ready(
        gps_state_s * const gps_state );


//
uint8_t gps_is_group_ready(
        const uint16_t group,
        const gps_state_s * const gps_state );


//
uint8_t gps_update(
        gps_state_s * const gps_state );




#endif	/* GPS_H */
