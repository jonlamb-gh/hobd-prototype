/**
 * @file obd.h
 * @brief TODO.
 *
 */




#ifndef OBD_H
#define	OBD_H




#include <inttypes.h>

#include "hobd.h"




//
#define OBD_GROUP_NONE_READY (0x0000)
#define OBD_GROUP_A_READY (1 << 0)


// ms
#define OBD_RX_WARN_TIMEOUT (5000UL)




// OBD message data group
typedef struct
{
    //
    //
    uint16_t ready_groups;
    //
    // GPS_GROUP_A_READY
//    struct
//    {
//        //
//        //
//        hobd_gps_time1_s time1;
//        //
//        //
//        hobd_gps_time2_s time2;
//    } group_a;
//    //
//    // GPS_GROUP_B_READY
//    struct
//    {
//        //
//        //
//        hobd_gps_pos_llh1_s pos_llh1;
//        //
//        //
//        hobd_gps_pos_llh2_s pos_llh2;
//        //
//        //
//        hobd_gps_pos_llh3_s pos_llh3;
//        //
//        //
//        hobd_gps_pos_llh4_s pos_llh4;
//    } group_b;
//    //
//    // GPS_GROUP_C_READY
//    struct
//    {
//        //
//        //
//        hobd_gps_baseline_ned1_s baseline_ned1;
//        //
//        //
//        hobd_gps_baseline_ned2_s baseline_ned2;
//        //
//        //
//        hobd_gps_baseline_ned3_s baseline_ned3;
//    } group_c;
} obd_data_s;




//
uint8_t obd_init( void );


//
void obd_disable( void );


//
void obd_enable( void );


//
void obd_set_group_ready(
        const uint16_t group );


//
void obd_clear_group_ready(
        const uint16_t group );


//
void obd_clear_all_group_ready( void );


//
uint8_t obd_is_group_ready(
        const uint16_t group );


//
uint8_t obd_update( void );




#endif	/* OBD_H */
