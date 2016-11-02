/**
 * @file hobd_uart.h
 * @brief Honda OBD Uart Protocol.
 *
 * @todo hobd protocol notes.
 *
 * Data rate: 10,400 bps
 *
 * Initialization sequence:
 *   -# pull k-line low for 70 ms
 *   -# return k-line to high state
 *   -# wait 120 ms
 *   -# send wake up packet - no response is expected
 *     - send {0xFE, 0x04, 0xFF, 0xFF}
 *   -# send initialization command packet - expect a response
 *     - send {0x72, 0x05, 0x00, 0xF0, 0x99}
 *     - expect {0x02, 0x04, 0x00, 0xFA}
 *
 * Checksum:
 * @todo - checksum notes.
 *
 */




#ifndef HOBD_UART_H
#define HOBD_UART_H




#include <inttypes.h>




//
#define HOBD_PACKET_SIZE_MAX (255)


//
#define HOBD_PACKET_TYPE_INVALID (0x00)


//
#define HOBD_PACKET_TYPE_QUERY (0x72)


//
#define HOBD_PACKET_TYPE_RESPONSE (0x02)


//
#define HOBD_PACKET_TYPE_WAKE_UP (0xFE)


//
#define HOBD_PACKET_SUBTYPE_WAKE_UP (0xFF)


//
#define HOBD_PACKET_SUBTYPE_INIT_COMMAND (0x00)


//
#define HOBD_PACKET_SUBTYPE_TABLE_SUBGROUP (0x72)


//
#define HOBD_PACKET_SUBTYPE_TABLE (0x71)


//
#define HOBD_INIT_COMMAND_DATA (0xF0)


//
#define HOBD_TABLE_0 (0x00)


//
#define HOBD_TABLE_16 (0x10)


//
#define HOBD_TABLE_32 (0x20)




//
typedef struct
{
    //
    //
    uint8_t type;
    //
    //
    uint8_t size;
    //
    //
    uint8_t subtype;
} hobd_packet_header;


//
typedef struct
{
    //
    //
    uint8_t type;
    //
    //
    uint8_t size;
    //
    //
    uint8_t subtype;
    //
    //
    uint8_t table;
    //
    //
    uint8_t register_offset;
    //
    //
    uint8_t register_cnt;
} hobd_table_query;


//
typedef struct
{
    //
    //
    uint8_t type;
    //
    //
    uint8_t size;
    //
    //
    uint8_t subtype;
    //
    //
    uint8_t table;
    //
    //
    uint8_t register_offset;
    //
    // register data follows after register_offset
} hobd_table_response;


//
typedef struct
{
    //
    //
    uint8_t type;
    //
    //
    uint8_t size;
    //
    //
    uint8_t subtype;
    //
    //
    uint8_t data;
} hobd_init_command;




#endif  /* HOBD_UART_H */
