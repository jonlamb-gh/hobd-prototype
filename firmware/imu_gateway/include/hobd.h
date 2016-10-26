/**
 * @file hobd.h
 * @brief TODO.
 *
 */




#ifndef HOBD_H
#define	HOBD_H




#include <inttypes.h>




//
#define HOBD_CAN_ID_HEARTBEAT_BASE (0x100)


// ms
#define HOBD_CAN_TX_INTERVAL_HEARTBEAT (50)


//
#define HOBD_CAN_ID_COMMAND (0x200)


//
#define HOBD_CAN_ID_RESPONSE (0x201)


//
#define HOBD_HEARTBEAT_STATE_INVALID (0x00)


//
#define HOBD_HEARTBEAT_STATE_INIT (0x01)


//
#define HOBD_HEARTBEAT_STATE_OK (0x02)


//
#define HOBD_HEARTBEAT_WARN_CANBUS (1 << 7)


//
#define HOBD_HEARTBEAT_WARN_HOBDBUS (1 << 6)




//
typedef struct
{
    //
    //
    uint8_t hardware_version : 4;
    //
    //
    uint8_t firmware_version : 4;
    //
    //
    uint8_t node_id;
    //
    //
    uint8_t state;
    //
    //
    uint8_t counter;
    //
    //
    uint16_t error_register;
    //
    //
    uint16_t warning_register;
} hobd_heartbeat_s;


//
typedef struct
{
    //
    //
    uint8_t id;
    //
    //
    uint8_t key;
    //
    //
    uint16_t data_0;
    //
    //
    uint32_t data_1;
} hobd_command_s;


//
typedef struct
{
    //
    //
    uint8_t cmd_id;
    //
    //
    uint8_t key;
    //
    //
    uint16_t data_0;
    //
    //
    uint32_t data_1;
} hobd_response_s;


/**
 * @brief GPS time 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    // local rx ms timestamp we got the GPS time. [millisecond]
    uint32_t rx_time;
    //
    // GPS time of week rounded to the nearest millisecond. [millisecond]
    uint32_t time_of_week;
} hobd_gps_time1_s;


/**
 * @brief GPS time 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    // GPS week number. [weeks]
    uint16_t week_number;
    //
    // Nanosecond residual of millisecond-rounded TOW (ranges from -500000 to 500000)
    int32_t residual;
    //
    //
    uint16_t reserved;
} hobd_gps_time2_s;




#endif	/* HOBD_H */
