/**
 * @file render.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gl_headers.h"
#include "math_util.h"
#include "render.h"




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




// *****************************************************
// public definitions
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
