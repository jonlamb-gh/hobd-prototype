/**
 * @file render_page5.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gl_headers.h"
#include "math_util.h"
#include "time_domain.h"
#include "render.h"
#include "signal_table.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************




// *****************************************************
// static declarations
// *****************************************************

//
void render_hobd_imu_utc_time1(
        const config_s * const config,
        const hobd_imu_utc_time1_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_imu_utc_time2(
        const config_s * const config,
        const hobd_imu_utc_time2_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_imu_rate_of_turn1(
        const config_s * const config,
        const hobd_imu_rate_of_turn1_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_imu_rate_of_turn2(
        const config_s * const config,
        const hobd_imu_rate_of_turn2_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );




// *****************************************************
// static definitions
// *****************************************************




// *****************************************************
// public definitions
// *****************************************************

//
void render_page5(
        const config_s * const config,
        st_state_s * const state )
{
    glPushMatrix();

    // get table pointers
    signal_table_s * const table0 = st_get_table_by_can_id(
            HOBD_CAN_ID_IMU_UTC_TIME1,
            state );

    signal_table_s * const table1 = st_get_table_by_can_id(
            HOBD_CAN_ID_IMU_UTC_TIME2,
            state );

    signal_table_s * const table2 = st_get_table_by_can_id(
            HOBD_CAN_ID_IMU_RATE_OF_TURN1,
            state );

    signal_table_s * const table3 = st_get_table_by_can_id(
            HOBD_CAN_ID_IMU_RATE_OF_TURN2,
            state );

    // render tables
    if( table0 != NULL )
    {
        render_table_base( table0, 5.0, 40.0 );
        render_hobd_imu_utc_time1( config, &table0->imu_utc_time1, 20.0, 80.0 );
    }

    if( table1 != NULL )
    {
        render_table_base( table1, 400.0, 40.0 );
        render_hobd_imu_utc_time2( config, &table1->imu_utc_time2, 415.0, 80.0 );
    }

    if( table2 != NULL )
    {
        render_table_base( table2, 5.0, 340.0 );
        render_hobd_imu_rate_of_turn1( config, &table2->imu_rate_of_turn1, 20.0, 380.0 );
    }

    if( table3 != NULL )
    {
        render_table_base( table3, 400.0, 340.0 );
        render_hobd_imu_rate_of_turn2( config, &table3->imu_rate_of_turn2, 415.0, 380.0 );
    }

    glPopMatrix();
}
