/**
 * @file render_hobd_heartbeat.c
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
void render_hobd_heartbeat(
        const config_s * const config,
        const hobd_heartbeat_s * const data,
        const GLdouble base_x,
        const GLdouble base_y )
{
    char string[512];
    GLdouble delta_y = 5.0;
    const GLdouble bound_x = 355.0;
    const GLdouble text_yoff = 15.0;
    const GLdouble text_xoff = 5.0;

    render_line(
            base_x,
            base_y,
            base_x + bound_x,
            base_y );

    snprintf(
            string,
            sizeof(string),
            "hardware_version                         : %lu",
            (unsigned long) data->hardware_version );

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

    snprintf(
            string,
            sizeof(string),
            "firmware_version                           : %lu",
            (unsigned long) data->firmware_version );

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

    delta_y += 15.0;

    snprintf(
            string,
            sizeof(string),
            "node_id                                       : 0x%02lX",
            (unsigned long) data->node_id );

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

    delta_y += 15.0;

    snprintf(
            string,
            sizeof(string),
            "state                                            : %lu",
            (unsigned long) data->state );

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

    delta_y += 15.0;

    snprintf(
            string,
            sizeof(string),
            "counter                                        : %lu",
            (unsigned long) data->counter );

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

    delta_y += 15.0;

    snprintf(
            string,
            sizeof(string),
            "warning_register                           : 0x%04lX",
            (unsigned long) data->warning_register );

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

    delta_y += 15.0;

    snprintf(
            string,
            sizeof(string),
            "error_register                                : 0x%04lX",
            (unsigned long) data->error_register );

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
