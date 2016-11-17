/**
 * @file render_hobd_imu_rate_of_turn1.c
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
#include "config.h"
#include "signal_table_def.h"




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
void render_hobd_imu_rate_of_turn1(
        const config_s * const config,
        const hobd_imu_rate_of_turn1_s * const data,
        const GLdouble base_x,
        const GLdouble base_y )
{
    char string[512];
    double value = 0.0;
    GLdouble delta_y = 5.0;
    const GLdouble bound_x = 355.0;
    const GLdouble text_yoff = 15.0;
    const GLdouble text_xoff = 5.0;

    render_line(
            base_x,
            base_y,
            base_x + bound_x,
            base_y );

    value = (double) data->x;
    snprintf(
            string,
            sizeof(string),
            "x                                                : %lf",
            value );

    render_text_2d(
            base_x + text_xoff,
            base_y + text_yoff,
            string,
            NULL );

    render_line(
            base_x,
            base_y + text_yoff + delta_y,
            base_x + bound_x,
            base_y + text_yoff + delta_y );

    delta_y += 15.0;

    value = (double) data->y;
    snprintf(
            string,
            sizeof(string),
            "y                                                : %lf",
            value );

    render_text_2d(
            base_x + text_xoff,
            base_y + text_yoff + delta_y,
            string,
            NULL );

    delta_y += 5.0;

    render_line(
            base_x,
            base_y + text_yoff + delta_y,
            base_x + bound_x,
            base_y + text_yoff + delta_y );
}
