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




// *****************************************************
// static definitions
// *****************************************************

//
void render_text_2d(
        const GLdouble cx,
        const GLdouble cy,
        const char * const text,
        const void * const font )
{
    // local vars
    const char *ptr = text;
    void *m_font = GLUT_BITMAP_HELVETICA_12;

    // set font
    if( font != NULL )
    {
        m_font = (void*) font;
    }

    // save state
    glPushMatrix();

    // start at
    glRasterPos2d( cx, cy );

    // render each char
    while( *ptr != '\0' )
    {
        glutBitmapCharacter( m_font, *ptr );
        ptr += 1;
    }

    // restore state
    glPopMatrix();
}


//
void render_line(
        const GLdouble x1,
        const GLdouble y1,
        const GLdouble x2,
        const GLdouble y2 )
{
    glBegin( GL_LINES );

    glVertex2d( x1, y1 );
    glVertex2d( x2, y2 );

    glEnd();
}


//
static void render_page_header(
        const unsigned long page_number )
{
    char string[256];
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

    const timestamp_ms now = time_get_timestamp();
    const timestamp_ms now_monotonic = time_get_monotonic_timestamp();

    render_line(
            base_x,
            base_y,
            base_x + bound_x,
            base_y );

    snprintf(
            string,
            sizeof(string),
            "%s",
            asctime_r( time_get_localtime( now ), date ) );

    render_text_2d(
            date_xoff,
            text_yoff,
            string,
            NULL );

    snprintf(
            string,
            sizeof(string),
            "ms: %llu",
            now );

    render_text_2d(
            mstime_xoff,
            text_yoff,
            string,
            NULL );

    snprintf(
            string,
            sizeof(string),
            "ms-mono: %llu",
            now_monotonic );

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
        const signal_table_s * const table )
{
    char string[256];
    char buffer_string[256];
    const GLdouble base_x = 5.0;
    const GLdouble base_y = 40.0;
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
void st_render(
        const signal_table_s * const signals,
        const unsigned long signals_len )
{
    glPushMatrix();

    glColor4d( 0.0, 0.0, 0.0, 1.0 );

    const signal_table_s test_table =
    {
        .rx_time = 123456789,
        .can_id = HOBD_CAN_ID_OBD2,
        .can_dlc = sizeof(hobd_obd2_s),
        .table_name = "OBD 2",
        .buffer = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }
    };

    render_page_header( 1 );

    render_table_base( &test_table );

//    glLineWidth( 1.0f );

    glPopMatrix();
}
