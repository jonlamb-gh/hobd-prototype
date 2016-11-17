/**
 * @file signal_table_def.h
 * @brief TODO.
 *
 */




#ifndef SIGNAL_TABLE_DEF_H
#define SIGNAL_TABLE_DEF_H




#include "time_domain.h"




// enforce 1 byte alignment so we can do linear packing
#pragma pack(push)
#pragma pack(1)


#include "hobd.h"




//
typedef struct
{
    //
    //
    timestamp_ms native_rx_time;
    //
    //
    timestamp_ms rx_time;
    //
    //
    timestamp_ms rx_time_mono;
    //
    //
    unsigned long can_id;
    //
    //
    unsigned long can_dlc;
    //
    //
    char table_name[ 256 ];
    //
    //
    union
    {
        //
        //
        unsigned char buffer[ 8 ];
        //
        //
        hobd_heartbeat_s heartbeat_obd_gateway;
        //
        //
        hobd_heartbeat_s heartbeat_imu_gateway;
        //
        //
        hobd_gps_time1_s gps_time1;
        //
        //
        hobd_gps_time2_s gps_time2;
        //
        //
        hobd_gps_dop1_s gps_dop1;
        //
        //
        hobd_gps_dop2_s gps_dop2;
        //
        //
        hobd_gps_pos_llh1_s gps_pos_llh1;
        //
        //
        hobd_gps_pos_llh2_s gps_pos_llh2;
        //
        //
        hobd_gps_pos_llh3_s gps_pos_llh3;
        //
        //
        hobd_gps_pos_llh4_s gps_pos_llh4;
        //
        //
        hobd_gps_baseline_ned1_s gps_baseline_ned1;
        //
        //
        hobd_gps_baseline_ned2_s gps_baseline_ned2;
        //
        //
        hobd_gps_baseline_ned3_s gps_baseline_ned3;
        //
        //
        hobd_gps_vel_ned1_s gps_vel_ned1;
        //
        //
        hobd_gps_vel_ned2_s gps_vel_ned2;
        //
        //
        hobd_gps_vel_ned3_s gps_vel_ned3;
        //
        //
        hobd_gps_heading1_s gps_heading1;
        //
        //
        hobd_gps_heading2_s gps_heading2;
        //
        //
        hobd_imu_sample_time_s imu_sample_time;
        //
        //
        hobd_imu_time1_s imu_time1;
        //
        //
        hobd_imu_time2_s imu_time2;
        //
        //
        hobd_imu_utc_time1_s imu_utc_time1;
        //
        //
        hobd_imu_utc_time2_s imu_utc_time2;
        //
        //
        hobd_imu_pos_llh1_s imu_pos_llh1;
        //
        //
        hobd_imu_pos_llh2_s imu_pos_llh2;
        //
        //
        hobd_imu_vel_nwu1_s imu_vel_nwu1;
        //
        //
        hobd_imu_vel_nwu2_s imu_vel_nwu2;
        //
        //
        hobd_imu_orient_quat1_s imu_orient_quat1;
        //
        //
        hobd_imu_orient_quat2_s imu_orient_quat2;
        //
        //
        hobd_imu_rate_of_turn1_s imu_rate_of_turn1;
        //
        //
        hobd_imu_rate_of_turn2_s imu_rate_of_turn2;
        //
        //
        hobd_imu_accel1_s imu_accel1;
        //
        //
        hobd_imu_accel2_s imu_accel2;
        //
        //
        hobd_imu_magf1_s imu_magf1;
        //
        //
        hobd_imu_magf2_s imu_magf2;
        //
        //
        hobd_obd_time_s obd_time;
        //
        //
        hobd_obd1_s obd1;
        //
        //
        hobd_obd2_s obd2;
        //
        //
        hobd_obd3_s obd3;
    };
} signal_table_s;


// restore alignment
#pragma pack(pop)




#endif /* SIGNAL_TABLE_DEF_H */
