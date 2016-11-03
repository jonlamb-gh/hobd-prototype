/**
 * @file display_manager.c
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
#include "display_manager.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************

//
static dm_context_s dm_context;




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static void on_close( void )
{
    if( dm_context.exit_signal_ptr != NULL )
    {
        (*dm_context.exit_signal_ptr) = 1;
    }

    dm_context.win_id = DM_WINDOW_ID_INVALID;
}

//
static void on_key(
        const unsigned char key,
        const int x,
        const int y )
{

}


//
static void on_special_key(
        const int key,
        const int x,
        const int y )
{

}


//
static void on_mouse(
        const int button,
        const int state,
        const int x,
        const int y )
{

}


//
static void on_mouse_motion(
        const int x,
        const int y )
{

}


//
static void on_resize(
        const int w,
        const int h )
{

}


//
static void on_draw( void )
{
    glutSwapBuffers();
}





// *****************************************************
// public definitions
// *****************************************************

//
dm_context_s * dm_get_context( void )
{
    return &dm_context;
}


//
int dm_init(
        const char * const window_title,
        const unsigned long window_width,
        const unsigned long window_height,
        sig_atomic_t * const exit_signal )
{
    int ret = 0;

    // zero global context
    memset( &dm_context, 0, sizeof(dm_context) );
    dm_context.win_id = DM_WINDOW_ID_INVALID;

    // pointer to user's exit signal
    dm_context.exit_signal_ptr = exit_signal;

    // set window data
    strncpy( dm_context.win_title, window_title, sizeof(dm_context.win_title) );
    dm_context.win_width = window_width;
    dm_context.win_height = window_height;

    // init GL
    glutInit( &dm_context.gl_argc, dm_context.gl_argv );

    // display mode, double buffers, RGB
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

    // win size
    glutInitWindowSize( dm_context.win_width, dm_context.win_height );

    // create display window
    dm_context.win_id = glutCreateWindow( dm_context.win_title );
    if( dm_context.win_id < 0 )
    {
        dm_context.win_id = DM_WINDOW_ID_INVALID;
        ret = 1;
    }

    if( ret == 0 )
    {
        // set callbacks
        glutCloseFunc( on_close );
        glutKeyboardFunc( on_key );
        glutSpecialFunc( on_special_key );
        glutMouseFunc( on_mouse );
        glutMotionFunc( on_mouse_motion );
        glutReshapeFunc( on_resize );
        glutDisplayFunc( on_draw );

        // set config flags
        glDisable( GL_DEPTH );
        glDisable( GL_LIGHTING );
        glShadeModel( GL_SMOOTH );
        glDisable( GL_DEPTH_TEST );

        // smoothness
        glEnable( GL_LINE_SMOOTH );
        glEnable( GL_POLYGON_SMOOTH );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

        // alpha blending config
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // main loop returns on window exit
        glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

        // clear the color buffer, background, to black, RGBA
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

        // signal redraw
        glutPostRedisplay();

        // process glut events
        glutMainLoopEvent();
    }

    return ret;
}


//
void dm_release( void )
{
    // signal GL exit
    glutExit();

    dm_context.win_id = DM_WINDOW_ID_INVALID;
}


//
void dm_update(
        timestamp_ms * const time_to_redraw )
{
    if( dm_context.win_id != DM_WINDOW_ID_INVALID )
    {
        const timestamp_ms redraw_interval = DM_REDRAW_INTERVAL;

        const timestamp_ms delta = time_get_since_monotonic(
                dm_context.last_redraw_time );

        if( delta >= redraw_interval )
        {
            if( time_to_redraw != NULL )
            {
                (*time_to_redraw) = 0;
            }

            // signal redraw
            glutPostRedisplay();

            // process glut events
            glutMainLoopEvent();

            dm_context.last_redraw_time = time_get_monotonic_timestamp();
        }
        else
        {
            if( time_to_redraw != NULL )
            {
                (*time_to_redraw) = (redraw_interval - delta);
            }
        }
    }
}
