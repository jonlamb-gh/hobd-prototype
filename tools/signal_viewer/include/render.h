/**
 * @file render.h
 * @brief TODO.
 *
 */




#ifndef RENDER_H
#define RENDER_H




#include "gl_headers.h"




//
void render_text_2d(
        const GLdouble cx,
        const GLdouble cy,
        const char * const text,
        const void * const font );


//
void render_line(
        const GLdouble x1,
        const GLdouble y1,
        const GLdouble x2,
        const GLdouble y2 );




#endif /* RENDER_H */
