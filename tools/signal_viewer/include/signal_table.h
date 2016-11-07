/**
 * @file signal_table.h
 * @brief TODO.
 *
 */




#ifndef SIGNAL_TABLE_H
#define SIGNAL_TABLE_H




#include "time_domain.h"
#include "can_frame.h"
#include "config.h"
#include "signal_table_def.h"




//
#define ST_SIGNAL_COUNT (38)


//
#define ST_PAGE_1 (0)
#define ST_PAGE_2 (1)
#define ST_PAGE_3 (2)




//
typedef struct
{
    //
    //
    timestamp_ms last_update;
    //
    //
    timestamp_ms last_update_mono;
    //
    //
    signal_table_s signal_tables[ ST_SIGNAL_COUNT ];
} st_state_s;




//
void st_init(
        const config_s * const config,
        st_state_s * const state );


//
void st_render(
        const config_s * const config,
        st_state_s * const state );


//
signal_table_s *st_get_table_by_can_id(
        const unsigned long can_id,
        st_state_s * const state );


//
void st_process_can_frame(
        const can_frame_s * const can_frame,
        const config_s * const config,
        st_state_s * const state );




#endif /* SIGNAL_TABLE_H */
