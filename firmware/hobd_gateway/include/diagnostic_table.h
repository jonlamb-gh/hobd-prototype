/**
 * @file diagnostic_table.h
 * @brief TODO.
 *
 */




#ifndef DIAGNOSTIC_TABLE_H
#define	DIAGNOSTIC_TABLE_H




#include <stdint.h>




//
#define DTABLE_MAX_REGISTERS (24)




//
typedef struct
{
    //
    //
    uint8_t id;
    //
    //
    uint32_t last_update;
    //
    //
    uint8_t register_offset;
    //
    //
    uint8_t register_cnt;
    //
    //
    uint8_t registers[DTABLE_MAX_REGISTERS];
} diagnostic_table;




//
void dtable_update(
        const uint8_t * const packet,
        const uint32_t rx_timestamp,
        diagnostic_table * const dtable );




#endif	/* DIAGNOSTIC_TABLE_H */
