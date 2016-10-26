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
 * @brief GPS time 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t rx_time; /*!< Local rx millisecond timestamp when the GPS time data was received. [milliseconds] */
    //
    //
    uint32_t time_of_week; /*!< GPS time of week rounded to the nearest millisecond. [milliseconds] */
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
    //
    uint16_t week_number; /*!< GPS week number. [weeks] */
    //
    //
    int32_t residual; /*!< Nanosecond residual of millisecond-rounded TOW (ranges from -500000 to 500000). [nanoseconds] */
    //
    //
    uint16_t reserved; /*!< Reserved data. */
} hobd_gps_time2_s;


/**
 * @brief GPS dilution of precision 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
    //
    //
    uint16_t gdop; /*!< Geometric dilution of precision. [0.01] */
    //
    //
    uint16_t pdop; /*!< Position dilution of precision. [0.01] */
} hobd_gps_dop1_s;


/**
 * @brief GPS dilution of precision 2 message.
 *
 * Message size (CAN frame DLC): 6 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint16_t tdop; /*!< Time dilution of precision. [0.01] */
    //
    //
    uint16_t hdop; /*!< Horizontal dilution of precision. [0.01] */
    //
    //
    uint16_t vdop; /*!< Vertical dilution of precision. [0.01] */
} hobd_gps_dop2_s;


/**
 * @brief GPS ECEF position 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
    //
    //
    uint16_t accuracy; /*!< Position accuracy estimate. [milimeters] */
    //
    //
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t flags; /*!< Reserved data. */
} hobd_gps_pos_ecef1_s;


/**
 * @brief GPS ECEF position 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint64_t x; /*!< ECEF X coordinate. [meters] */
} hobd_gps_pos_ecef2_s;


/**
 * @brief GPS ECEF position 3 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint64_t y; /*!< ECEF Y coordinate. [meters] */
} hobd_gps_pos_ecef3_s;


/**
 * @brief GPS ECEF position 4 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint64_t z; /*!< ECEF Z coordinate. [meters] */
} hobd_gps_pos_ecef4_s;


/**
 * @brief GPS baseline ECEF position 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
    //
    //
    uint16_t accuracy; /*!< Position accuracy estimate. [millimeters] */
    //
    //
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t flags; /*!< Reserved data. */
} hobd_gps_baseline_ecef1_s;


/**
 * @brief GPS baseline ECEF position 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t x; /*!< Baseline ECEF X coordinate. [millimeters] */
    //
    //
    int32_t y; /*!< Baseline ECEF X coordinate. [millimeters] */
} hobd_gps_baseline_ecef2_s;


/**
 * @brief GPS baseline ECEF position 3 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t z; /*!< Baseline ECEF Z coordinate. [millimeters] */
} hobd_gps_baseline_ecef3_s;


/**
 * @brief GPS ECEF velocity 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
    //
    //
    uint16_t accuracy; /*!< Position accuracy estimate. [milimeters/second] */
    //
    //
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t flags; /*!< Reserved data. */
} hobd_gps_vel_ecef1_s;


/**
 * @brief GPS ECEF velocity 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t x; /*!< ECEF X coordinate. [millimeters/second] */
    //
    //
    int32_t y; /*!< ECEF X coordinate. [millimeters/second] */
} hobd_gps_vel_ecef2_s;


/**
 * @brief GPS ECEF velocity 3 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t z; /*!< ECEF Z coordinate. [millimeters/second] */
} hobd_gps_vel_ecef3_s;


/**
 * @brief GPS heading 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
    //
    //
    uint32_t heading; /*!< Heading relative to True North. [millidegrees] */
} hobd_gps_heading1_s;


/**
 * @brief GPS heading 2 message.
 *
 * Message size (CAN frame DLC): 2 bytes
 * CAN frame ID: TODO
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t flags; /*!< Reserved data. */
} hobd_gps_heading2_s;




#endif	/* HOBD_H */
