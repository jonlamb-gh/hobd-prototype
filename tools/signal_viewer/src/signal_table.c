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
static void draw_table_base(
        const signal_table_s * const table )
{
    const GLdouble base_x = 5.0;
    const GLdouble base_y = 5.0;

    char string[256];
    const GLdouble bound_x = 300.0;
    const GLdouble table_name_xoff = 5.0;
    const GLdouble table_name_yoff = 15.0;

    render_line(
            base_x,
            base_y,
            base_x + bound_x,
            base_y );

    snprintf(
            string,
            sizeof(string),
            "ID: 0x%lX (%lu) - DLC %lu - '%s'",
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

    snprintf(
            string,
            sizeof(string),
            "Rx (ms): %llu",
            table->rx_time );

    render_text_2d(
            base_x + table_name_xoff,
            base_y + 2 * table_name_yoff,
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
        .rx_time = 1,
        .can_id = 0x100,
        .can_dlc = 8,
        .table_name = "test-table"
    };

    draw_table_base( &test_table );

    glPopMatrix();
}
