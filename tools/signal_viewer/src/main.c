/**
 * @file main.c
 * @brief TODO.
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "math_util.h"
#include "display_manager.h"




// *****************************************************
// static global types/macros
// *****************************************************

//
#define WINDOW_WIDTH (1200UL)


//
#define WINDOW_HEIGHT (800UL)


//
#define WINDOW_TITLE "HOBD CAN Signal Viewer"




// *****************************************************
// static global data
// *****************************************************

/**
 * @brief Flag indicating exit signal was caught.
 *
 */
static sig_atomic_t global_exit_signal = 0;




// *****************************************************
// static declarations
// *****************************************************


/**
 * @brief Signal handler.
 *
 * @param [in] signal Signal value to handle.
 *
 */
static void sig_handler( int signal );




// *****************************************************
// static definitions
// *****************************************************

//
static void sig_handler( int sig )
{
    if( sig == SIGINT )
    {
        global_exit_signal = 1;
    }
}




// *****************************************************
// public definitions
// *****************************************************


int main( int argc, char **argv )
{
    // hook up the control-c signal handler, sets exit signaled flag
    signal( SIGINT, sig_handler );

    // allow signals to interrupt
    siginterrupt( SIGINT, 1 );

    // init display manager
    const int dm_init_status = dm_init(
            WINDOW_TITLE,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            &global_exit_signal );
    if( dm_init_status != 0 )
    {
        printf( "dm_init return %d\n", dm_init_status );
        global_exit_signal = 1;
    }

    // wait for user to control-c
    while( global_exit_signal == 0 )
    {
        timestamp_ms time_to_redraw = 0;

        // update display manager
        dm_update( &time_to_redraw );

        // TODO - adjust this mechanism as IO gets added
        if( time_to_redraw > 0 )
        {
            time_sleep_ms( m_min(time_to_redraw,5) );
        }
    }

    // release display manager
    dm_release();

	return EXIT_SUCCESS;
}
