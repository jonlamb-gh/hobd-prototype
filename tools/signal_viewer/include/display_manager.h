/**
 * @file display_manager.h
 * @brief TODO.
 *
 */




#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H




#include <signal.h>
#include "gl_headers.h"
#include "time_domain.h"




//
#define DM_WINDOW_ID_INVALID (-1)


// 33 ms ~= 30 FPS
#define DM_REDRAW_INTERVAL (33ULL)




/**
 * @brief Display manager context data.
 *
 */
typedef struct
{
    //
    //
    int gl_argc; /*!< GL argument count. */
    //
    //
    char **gl_argv; /*!< GL arugment list. */
    //
    //
    GLenum gl_error; /*!< GL error value. */
    //
    //
    int win_id; /*!< Window identifier.
                 * Value \ref GUI_WINDOW_ID_INVALID means invalid. */
    //
    //
    unsigned long win_width; /*!< Window width. [pixels] */
    //
    //
    unsigned long win_height; /*!< Window height. [pixels] */
    //
    //
    char win_title[ 1024 ]; /*!< Window title string. */
    //
    // monotonic
    timestamp_ms last_redraw_time;
    //
    //
    sig_atomic_t *exit_signal_ptr;
} dm_context_s;




//
dm_context_s * dm_get_context( void );


//
int dm_init(
        const char * const window_title,
        const unsigned long window_width,
        const unsigned long window_height,
        sig_atomic_t * const exit_signal );


//
void dm_release( void );


//
void dm_update(
        timestamp_ms * const time_to_redraw );




#endif /* DISPLAY_MANAGER_H */
