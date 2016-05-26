/**
 * @file diagnostic_table.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "hobd.h"
#include "diagnostic_table.h"




// *****************************************************
// static global types/macros
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
void dtable_update(
        const uint8_t * const packet,
        const uint32_t timestamp,
        diagnostic_table * const dtable )
{
    // cast response
    const hobd_table_response * const resp =
            (hobd_table_response*) packet;

    // set table ID
    dtable->id = resp->table;

    // update timestamp
    dtable->last_update = timestamp;

    // register offset position
    dtable->register_offset = resp->register_offset;

    // number of registers
    dtable->register_cnt = (resp->size - sizeof(*resp) - 1);

    // copy register data
    memcpy( dtable->registers, &packet[ sizeof(*resp) ], dtable->register_cnt );
}
