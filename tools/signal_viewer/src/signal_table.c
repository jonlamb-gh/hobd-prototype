/**
 * @file signal_table.c
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
#include "can_frame.h"
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
void render_hobd_obd_time(
        const config_s * const config,
        const hobd_obd_time_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_obd1(
        const config_s * const config,
        const hobd_obd1_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_obd2(
        const config_s * const config,
        const hobd_obd2_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );


//
void render_hobd_obd3(
        const config_s * const config,
        const hobd_obd3_s * const data,
        const GLdouble base_x,
        const GLdouble base_y );




// *****************************************************
// static definitions
// *****************************************************

//
static signal_table_s *get_table_by_can_id(
        const unsigned long can_id,
        st_state_s * const state )
{
    signal_table_s *table = NULL;

    unsigned long idx = 0;
    for( idx = 0; (idx < ST_SIGNAL_COUNT) && (table == NULL); idx += 1 )
    {
        if( state->signal_tables[ idx ].can_id == can_id )
        {
            table = &state->signal_tables[ idx ];
        }
    }

    return table;
}


//
static void render_page_header(
        const config_s * const config,
        const st_state_s * const state,
        const unsigned long page_number )
{
    char string[512];
    char date[64];
    const GLdouble base_x = 0.0;
    const GLdouble base_y = 25.0;
    const GLdouble bound_x = 1200.0;
    const GLdouble text_yoff = 15.0;
    const GLdouble date_xoff = 5.0;
    const GLdouble mstime_xoff = 200.0;
    const GLdouble monotime_xoff = 400.0;
    const GLdouble page_xoff = 600.0;

    glLineWidth( 2.0f );

    render_line(
            base_x,
            base_y,
            base_x + bound_x,
            base_y );

    snprintf(
            string,
            sizeof(string),
            "%s",
            asctime_r( time_get_localtime( state->last_update ), date ) );

    render_text_2d(
            date_xoff,
            text_yoff,
            string,
            NULL );

    snprintf(
            string,
            sizeof(string),
            "ms: %llu",
            state->last_update );

    render_text_2d(
            mstime_xoff,
            text_yoff,
            string,
            NULL );

    snprintf(
            string,
            sizeof(string),
            "ms-mono: %llu",
            state->last_update_mono );

    render_text_2d(
            monotime_xoff,
            text_yoff,
            string,
            NULL );

    snprintf(
            string,
            sizeof(string),
            "Page %lu",
            page_number );

    render_text_2d(
            page_xoff,
            text_yoff,
            string,
            NULL );
}


//
static void render_table_base(
        const config_s * const config,
        const signal_table_s * const table,
        const GLdouble base_x,
        const GLdouble base_y )
{
    char string[512];
    char buffer_string[512];
    const GLdouble bound_x = 370.0;
    const GLdouble table_name_xoff = 5.0;
    const GLdouble table_name_yoff = 15.0;
    const GLdouble text_delta_y = 20.0;
    const GLdouble text_col_b_xoff = 160.0;

    glLineWidth( 2.0f );

    render_line(
            base_x,
            base_y,
            base_x + bound_x,
            base_y );

    snprintf(
            string,
            sizeof(string),
            "ID: 0x%04lX (%lu) - DLC %lu - '%s'",
            table->can_id,
            table->can_id,
            table->can_dlc,
            table->table_name );

    render_text_2d(
            base_x + table_name_xoff,
            base_y + table_name_yoff,
            string,
            NULL );

    render_line(
            base_x,
            base_y + table_name_yoff + 5,
            base_x + bound_x,
            base_y + table_name_yoff + 5);

    if( table->can_dlc == 0 )
    {
        snprintf(
                buffer_string,
                sizeof(buffer_string),
                " NA" );
    }
    else
    {
        memset( buffer_string, 0, sizeof(buffer_string) );

        unsigned long idx = 0;
        for( idx = 0; idx < table->can_dlc; idx += 1 )
        {
            char hex_string[4];

            snprintf(
                    hex_string,
                    sizeof(hex_string),
                    " %02X",
                    (unsigned int) table->buffer[ table->can_dlc - idx - 1] );

            strncat(
                    buffer_string,
                    hex_string,
                    sizeof(buffer_string) );
        }
    }

    snprintf(
            string,
            sizeof(string),
            "Rx (ms): %llu",
            table->rx_time );

    render_text_2d(
            base_x + table_name_xoff,
            base_y + table_name_yoff + text_delta_y,
            string,
            NULL );

    snprintf(
            string,
            sizeof(string),
            "data[M-L]:%s",
            buffer_string );

    render_text_2d(
            base_x + table_name_xoff + text_col_b_xoff,
            base_y + table_name_yoff + text_delta_y,
            string,
            NULL );
}




// *****************************************************
// public definitions
// *****************************************************

//
void st_init(
        const config_s * const config,
        st_state_s * const state )
{
    {
        signal_table_s * const table = &state->signal_tables[ 0 ];

        table->can_id = HOBD_CAN_ID_OBD_TIME;
        table->can_dlc = (unsigned long) sizeof( table->obd_time );
        snprintf(
                table->table_name,
                sizeof(table->table_name),
                "OBD Time" );
    }

    {
        signal_table_s * const table = &state->signal_tables[ 1 ];

        table->can_id = HOBD_CAN_ID_OBD1;
        table->can_dlc = (unsigned long) sizeof( table->obd1 );
        snprintf(
                table->table_name,
                sizeof(table->table_name),
                "OBD 1" );
    }

    {
        signal_table_s * const table = &state->signal_tables[ 2 ];

        table->can_id = HOBD_CAN_ID_OBD2;
        table->can_dlc = (unsigned long) sizeof( table->obd2 );
        snprintf(
                table->table_name,
                sizeof(table->table_name),
                "OBD 2" );
    }

    {
        signal_table_s * const table = &state->signal_tables[ 3 ];

        table->can_id = HOBD_CAN_ID_OBD3;
        table->can_dlc = (unsigned long) sizeof( table->obd3 );
        snprintf(
                table->table_name,
                sizeof(table->table_name),
                "OBD 3" );
    }
}


//
void st_render(
        const config_s * const config,
        st_state_s * const state )
{
    glPushMatrix();

    glColor4d( 0.0, 0.0, 0.0, 1.0 );

    if( config->freeze_frame_enabled == FALSE )
    {
        state->last_update = time_get_timestamp();
        state->last_update_mono = time_get_monotonic_timestamp();
    }

    // get table pointers
    signal_table_s * const table0 = get_table_by_can_id(
            HOBD_CAN_ID_OBD_TIME,
            state );

    signal_table_s * const table1 = get_table_by_can_id(
            HOBD_CAN_ID_OBD1,
            state );

    signal_table_s * const table2 = get_table_by_can_id(
            HOBD_CAN_ID_OBD2,
            state );

    signal_table_s * const table3 = get_table_by_can_id(
            HOBD_CAN_ID_OBD3,
            state );

    // draw tables
    render_page_header( config, state, 1 );

    if( table0 != NULL )
    {
        render_table_base( config, table0, 5.0, 40.0 );
        render_hobd_obd_time( config, &table0->obd_time, 20.0, 80.0 );
    }

    if( table1 != NULL )
    {
        render_table_base( config, table1, 400.0, 40.0 );
        render_hobd_obd1( config, &table1->obd1, 415.0, 80.0 );
    }

    if( table2 != NULL )
    {
        render_table_base( config, table2, 5.0, 340.0 );
        render_hobd_obd2( config, &table2->obd2, 20.0, 380.0 );
    }

    if( table3 != NULL )
    {
        render_table_base( config, table3, 400.0, 340.0 );
        render_hobd_obd3( config, &table3->obd3, 415.0, 380.0 );
    }

    glPopMatrix();
}


//
void st_process_can_frame(
        const can_frame_s * const can_frame,
        const config_s * const config,
        st_state_s * const state )
{
    // TODO
}
