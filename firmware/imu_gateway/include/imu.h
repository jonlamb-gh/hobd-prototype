/**
 * @file imu.h
 * @brief TODO.
 *
 */




#ifndef IMU_H
#define	IMU_H




#include <inttypes.h>

#include "hobd.h"




//
#define IMU_GROUP_NONE_READY (0x0000)
#define IMU_GROUP_A_READY (1 << 0)
#define IMU_GROUP_B_READY (1 << 1)
#define IMU_GROUP_C_READY (1 << 2)
#define IMU_GROUP_D_READY (1 << 3)
#define IMU_GROUP_E_READY (1 << 4)
#define IMU_GROUP_F_READY (1 << 5)
#define IMU_GROUP_G_READY (1 << 6)
#define IMU_GROUP_H_READY (1 << 7)
#define IMU_GROUP_I_READY (1 << 8)
#define IMU_GROUP_J_READY (1 << 9)


// ms
#define IMU_FIX_WARN_TIMEOUT (5000UL)




// IMU message data group
typedef struct
{
    //
    //
    uint16_t ready_groups;
    //
    // IMU_GROUP_A_READY
    struct
    {
        //
        //
        hobd_imu_sample_time_s sample_time;
    } group_a;
    //
    // IMU_GROUP_B_READY
    struct
    {
        //
        //
        hobd_imu_time1_s time1;
        //
        //
        hobd_imu_time2_s time2;
    } group_b;
    //
    // IMU_GROUP_C_READY
    struct
    {
        //
        //
        hobd_imu_utc_time1_s utc_time1;
        //
        //
        hobd_imu_utc_time2_s utc_time2;
    } group_c;
    //
    // IMU_GROUP_D_READY
    struct
    {
        //
        //
        hobd_imu_orient_quat1_s orient_quat1;
        //
        //
        hobd_imu_orient_quat2_s orient_quat2;
    } group_d;
    //
    // IMU_GROUP_E_READY
    struct
    {
        //
        //
        hobd_imu_rate_of_turn1_s rate_of_turn1;
        //
        //
        hobd_imu_rate_of_turn2_s rate_of_turn2;
    } group_e;
    //
    // IMU_GROUP_F_READY
    struct
    {
        //
        //
        hobd_imu_accel1_s accel1;
        //
        //
        hobd_imu_accel2_s accel2;
    } group_f;
    //
    // IMU_GROUP_G_READY
    struct
    {
        //
        //
        hobd_imu_magf1_s magf1;
        //
        //
        hobd_imu_magf2_s magf2;
    } group_g;
    //
    // IMU_GROUP_H_READY
    struct
    {
        //
        //
        hobd_imu_pos_llh1_s pos_llh1;
    } group_h;
    //
    // IMU_GROUP_I_READY
    struct
    {
        //
        //
        hobd_imu_pos_llh2_s pos_llh2;
    } group_i;
    //
    // IMU_GROUP_J_READY
    struct
    {
        //
        //
        hobd_imu_vel_nwu1_s vel_nwu1;
        //
        //
        hobd_imu_vel_nwu2_s vel_nwu2;
    } group_j;
} imu_data_s;




//
uint8_t imu_init( void );


//
void imu_disable( void );


//
void imu_enable( void );


//
void imu_set_group_ready(
        const uint16_t group );


//
void imu_clear_group_ready(
        const uint16_t group );


//
void imu_clear_all_group_ready( void );


//
uint8_t imu_is_group_ready(
        const uint16_t group );


//
uint8_t imu_update( void );




#endif	/* IMU_H */
