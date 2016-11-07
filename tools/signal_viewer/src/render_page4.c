/**
 * @file render_page4.c
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
void render_hobd_gps_baseline_ned1(
        const config_s * const config,
        const hobd_gps_baseline_ned1_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_gps_pos_llh1(
        const config_s * const config,
        const hobd_gps_pos_llh1_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_gps_pos_llh2(
        const config_s * const config,
        const hobd_gps_pos_llh2_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );

//
void render_hobd_gps_pos_llh3(
        const config_s * const config,
        const hobd_gps_pos_llh3_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_gps_pos_llh4(
        const config_s * const config,
        const hobd_gps_pos_llh4_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );




// *****************************************************
// static definitions
// *****************************************************




// *****************************************************
// public definitions
// *****************************************************

//
void render_page4(
        const config_s * const config,
        st_state_s * const state )
{
    glPushMatrix();

    // get table pointers
    signal_table_s * const table0 = st_get_table_by_can_id(
            HOBD_CAN_ID_GPS_BASELINE_NED1,
            state );

    signal_table_s * const table1 = st_get_table_by_can_id(
            HOBD_CAN_ID_GPS_POS_LLH1,
            state );

    signal_table_s * const table2 = st_get_table_by_can_id(
            HOBD_CAN_ID_GPS_POS_LLH2,
            state );

    signal_table_s * const table3 = st_get_table_by_can_id(
            HOBD_CAN_ID_GPS_POS_LLH3,
            state );

    signal_table_s * const table4 = st_get_table_by_can_id(
            HOBD_CAN_ID_GPS_POS_LLH4,
            state );

    // render tables
    if( table0 != NULL )
    {
        render_table_base( table0, 5.0, 40.0 );
        render_hobd_gps_baseline_ned1( config, &table0->gps_baseline_ned1, 20.0, 80.0 );
    }

    if( table1 != NULL )
    {
        render_table_base( table1, 400.0, 40.0 );
        render_hobd_gps_pos_llh1( config, &table1->gps_pos_llh1, 415.0, 80.0 );
    }

    if( table2 != NULL )
    {
        render_table_base( table2, 5.0, 340.0 );
        render_hobd_gps_pos_llh2( config, &table2->gps_pos_llh2, 20.0, 380.0 );
    }

    if( table3 != NULL )
    {
        render_table_base( table3, 400.0, 340.0 );
        render_hobd_gps_pos_llh3( config, &table3->gps_pos_llh3, 415.0, 380.0 );
    }

    if( table4 != NULL )
    {
        render_table_base( table4, 5.0, 460.0 );
        render_hobd_gps_pos_llh4( config, &table2->gps_pos_llh4, 20.0, 500.0 );
    }

    glPopMatrix();
}
