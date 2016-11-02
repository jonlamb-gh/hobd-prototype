/**
 * @file hobd.h
 * @brief TODO.
 *
 */




#ifndef HOBD_H
#define	HOBD_H




#include <inttypes.h>




//
#define HOBD_CAN_ID_HEARTBEAT_BASE (0x010)


// ms
#define HOBD_CAN_TX_INTERVAL_HEARTBEAT (50)


//
#define HOBD_CAN_ID_COMMAND (0x020)


//
#define HOBD_CAN_ID_RESPONSE (0x021)


// GPS ID's
#define HOBD_CAN_ID_GPS_TIME1 (0x040)
#define HOBD_CAN_ID_GPS_TIME2 (0x041)
#define HOBD_CAN_ID_GPS_POS_LLH1 (0x042)
#define HOBD_CAN_ID_GPS_POS_LLH2 (0x043)
#define HOBD_CAN_ID_GPS_POS_LLH3 (0x044)
#define HOBD_CAN_ID_GPS_POS_LLH4 (0x045)
#define HOBD_CAN_ID_GPS_BASELINE_NED1 (0x046)
#define HOBD_CAN_ID_GPS_BASELINE_NED2 (0x047)
#define HOBD_CAN_ID_GPS_BASELINE_NED3 (0x048)
#define HOBD_CAN_ID_GPS_VEL_NED1 (0x049)
#define HOBD_CAN_ID_GPS_VEL_NED2 (0x04A)
#define HOBD_CAN_ID_GPS_VEL_NED3 (0x04B)
#define HOBD_CAN_ID_GPS_HEADING1 (0x04C)
#define HOBD_CAN_ID_GPS_HEADING2 (0x04D)
#define HOBD_CAN_ID_GPS_DOP1 (0x04E)
#define HOBD_CAN_ID_GPS_DOP2 (0x04F)


// IMU ID's
#define HOBD_CAN_ID_IMU_SAMPLE_TIME (0x060)
#define HOBD_CAN_ID_IMU_TIME1 (0x061)
#define HOBD_CAN_ID_IMU_TIME2 (0x062)
#define HOBD_CAN_ID_IMU_UTC_TIME1 (0x063)
#define HOBD_CAN_ID_IMU_UTC_TIME2 (0x064)
#define HOBD_CAN_ID_IMU_POS_LLH1 (0x065)
#define HOBD_CAN_ID_IMU_POS_LLH2 (0x066)
#define HOBD_CAN_ID_IMU_VEL_NED1 (0x067)
#define HOBD_CAN_ID_IMU_VEL_NED2 (0x068)
#define HOBD_CAN_ID_IMU_ORIENT_QUAT1 (0x069)
#define HOBD_CAN_ID_IMU_ORIENT_QUAT2 (0x06A)
#define HOBD_CAN_ID_IMU_RATE_OF_TURN1 (0x06B)
#define HOBD_CAN_ID_IMU_RATE_OF_TURN2 (0x06C)
#define HOBD_CAN_ID_IMU_ACCEL1 (0x06D)
#define HOBD_CAN_ID_IMU_ACCEL2 (0x06E)
#define HOBD_CAN_ID_IMU_MAGF1 (0x06F)
#define HOBD_CAN_ID_IMU_MAGF2 (0x070)


// OBD ID's
#define HOBD_CAN_ID_OBD_TIME (0x080)
#define HOBD_CAN_ID_OBD1 (0x081)
#define HOBD_CAN_ID_OBD2 (0x082)


//
#define HOBD_HEARTBEAT_STATE_INVALID (0x00)
#define HOBD_HEARTBEAT_STATE_INIT (0x01)
#define HOBD_HEARTBEAT_STATE_OK (0x02)
#define HOBD_HEARTBEAT_STATE_ERROR (0x03)


//
#define HOBD_HEARTBEAT_WARN_CANBUS (1 << 6)
#define HOBD_HEARTBEAT_WARN_IMUBUS (1 << 7)
#define HOBD_HEARTBEAT_WARN_GPSBUS (1 << 8)
#define HOBD_HEARTBEAT_WARN_OBDBUS (1 << 9)
#define HOBD_HEARTBEAT_WARN_NO_GPS_FIX (1 << 10)
#define HOBD_HEARTBEAT_WARN_NO_IMU_FIX (1 << 11)
#define HOBD_HEARTBEAT_WARN_NO_OBD_ECU (1 << 12)


//
#define HOBD_HEARTBEAT_ERROR_OBD_RX_OVERFLOW (1 << 4)
#define HOBD_HEARTBEAT_ERROR_IMU_RX_OVERFLOW (1 << 5)
#define HOBD_HEARTBEAT_ERROR_CANBUS (1 << 6)
#define HOBD_HEARTBEAT_ERROR_IMUBUS (1 << 7)
#define HOBD_HEARTBEAT_ERROR_GPSBUS (1 << 8)
#define HOBD_HEARTBEAT_ERROR_OBDBUS (1 << 9)
#define HOBD_HEARTBEAT_ERROR_GPS_ANT1 (1 << 10)
#define HOBD_HEARTBEAT_ERROR_GPS_ANT2 (1 << 11)
#define HOBD_HEARTBEAT_ERROR_GPS_STATUS (1 << 12)
#define HOBD_HEARTBEAT_ERROR_IMU_STATUS (1 << 13)


//
#define HOBD_GPS_FIX_MODE_SPP (0)
#define HOBD_GPS_FIX_MODE_RTK_FLOAT (1)
#define HOBD_GPS_FIX_MODE_RTK_FIXED (2)


//
#define HOBD_GPS_HEIGHT_MODE_ELLIPSOID (0)
#define HOBD_GPS_HEIGHT_MODE_MEANSEA (1)




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
    uint16_t warning_register;
    //
    //
    uint16_t error_register;
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
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_TIME1
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
 * Message size (CAN frame DLC): 7 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_TIME2
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
    uint8_t flags; /*!< Reserved data. */
} hobd_gps_time2_s;


/**
 * @brief GPS dilution of precision 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_DOP1
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
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_DOP1
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
 * Message size (CAN frame DLC): 6 bytes
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
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t fix_mode : 2; /*!< Fix mode.
                           * See \ref HOBD_GPS_FIX_MODE_SPP. */
    //
    //
    uint8_t flags : 6; /*!< Reserved data. */
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
    double x; /*!< ECEF X coordinate. [meters] */
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
    double y; /*!< ECEF Y coordinate. [meters] */
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
    double z; /*!< ECEF Z coordinate. [meters] */
} hobd_gps_pos_ecef4_s;


/**
 * @brief GPS geodetic position 1 message.
 *
 * Message size (CAN frame DLC): 6 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_POS_LLH1
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
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t fix_mode : 2; /*!< Fix mode.
                           * See \ref HOBD_GPS_FIX_MODE_SPP. */
    //
    //
    uint8_t height_mode : 1; /*!< Height mode.
                              * See \ref HOBD_GPS_HEIGHT_MODE_ELLIPSOID. */
    //
    //
    uint8_t flags : 5; /*!< Reserved data. */
} hobd_gps_pos_llh1_s;


/**
 * @brief GPS geodetic position 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_POS_LLH2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    double latitude; /*!< Latitude. [degrees] */
} hobd_gps_pos_llh2_s;


/**
 * @brief GPS geodetic position 3 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_POS_LLH3
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    double longitude; /*!< Longitude. [degrees] */
} hobd_gps_pos_llh3_s;


/**
 * @brief GPS geodetic position 4 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_POS_LLH4
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    double height; /*!< Height. [meters] */
} hobd_gps_pos_llh4_s;


/**
 * @brief GPS baseline ECEF position 1 message.
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
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
    //
    //
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t fix_mode : 2; /*!< Fix mode.
                           * See \ref HOBD_GPS_FIX_MODE_SPP. */
    //
    //
    uint8_t flags : 6; /*!< Reserved data. */
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
 * @brief GPS baseline NED position 1 message.
 *
 * Message size (CAN frame DLC): 6 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_BASELINE_NED1
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
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t fix_mode : 2; /*!< Fix mode.
                           * See \ref HOBD_GPS_FIX_MODE_SPP. */
    //
    //
    uint8_t flags : 6; /*!< Reserved data. */
} hobd_gps_baseline_ned1_s;


/**
 * @brief GPS baseline NED position 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_BASELINE_NED2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t north; /*!< Baseline North coordinate. [millimeters] */
    //
    //
    int32_t east; /*!< Baseline East coordinate. [millimeters] */
} hobd_gps_baseline_ned2_s;


/**
 * @brief GPS baseline NED position 3 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_BASELINE_NED3
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t down; /*!< Baseline Down coordinate. [millimeters] */
} hobd_gps_baseline_ned3_s;


/**
 * @brief GPS ECEF velocity 1 message.
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
    uint32_t time_of_week; /*!< GPS time of week. [milliseconds] */
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
 * @brief GPS NED velocity 1 message.
 *
 * Message size (CAN frame DLC): 6 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_VEL_NED1
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
    uint8_t num_sats; /*!< Number of satellites used in the solution. */
    //
    //
    uint8_t flags; /*!< Reserved data. */
} hobd_gps_vel_ned1_s;


/**
 * @brief GPS NED velocity 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_VEL_NED2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t north; /*!< Velocity North coordinate. [millimeters/second] */
    //
    //
    int32_t east; /*!< Velocity East coordinate. [millimeters/second] */
} hobd_gps_vel_ned2_s;


/**
 * @brief GPS NED velocity 3 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_VEL_NED3
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    int32_t down; /*!< Velocity Down coordinate. [millimeters/second] */
} hobd_gps_vel_ned3_s;


/**
 * @brief GPS heading 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_HEADING1
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
 * CAN frame ID: \ref HOBD_CAN_ID_GPS_HEADING2
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


/**
 * @brief IMU sample time message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_SAMPLE_TIME
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t rx_time; /*!< Local rx millisecond timestamp when the IMU time data was received. [milliseconds] */
    //
    //
    uint32_t sample_time;
} hobd_imu_sample_time_s;


/**
 * @brief IMU time 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_TIME1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t rx_time; /*!< Local rx millisecond timestamp when the IMU UTC time data was received. [milliseconds] */
    //
    //
    uint16_t week_number; /*!< GPS week number. [weeks] */
    //
    //
    uint8_t gps_fix_type;
    //
    //
    uint8_t flags;
} hobd_imu_time1_s;


/**
 * @brief IMU time 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_TIME2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t time_of_week; /*!< GPS time of week rounded to the nearest millisecond. [milliseconds] */
    //
    //
    int32_t residual; /*!< Nanosecond residual of millisecond-rounded TOW (ranges from -500000 to 500000). [nanoseconds] */
} hobd_imu_time2_s;


/**
 * @brief IMU UTC time 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_UTC_TIME1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t rx_time; /*!< Local rx millisecond timestamp when the IMU UTC time data was received. [milliseconds] */
    //
    //
    uint8_t flags : 7;
    //
    //
    uint8_t gps_fix : 1;
    //
    //
    uint16_t year;
    //
    //
    uint8_t month;
} hobd_imu_utc_time1_s;


/**
 * @brief IMU UTC time 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_UTC_TIME2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint8_t day;
    //
    //
    uint8_t hour;
    //
    //
    uint8_t min;
    //
    //
    uint8_t sec;
    //
    //
    uint32_t nanosec;
} hobd_imu_utc_time2_s;


/**
 * @brief IMU geodetic position 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_POS_LLH1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float latitude; /*!< Latitude. [degrees] */
    //
    //
    float longitude; /*!< Longitude. [degrees] */
} hobd_imu_pos_llh1_s;


/**
 * @brief IMU geodetic position 2 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_POS_LLH2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float height; /*!< Height. [meters] */
} hobd_imu_pos_llh2_s;


/**
 * @brief IMU NED velocity 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_VEL_NED1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float north; /*!< Velocity North coordinate. [meters/second] */
    //
    //
    float east; /*!< Velocity East coordinate. [meters/second] */
} hobd_imu_vel_ned1_s;


/**
 * @brief IMU NED velocity 2 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_VEL_NED2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float down; /*!< Velocity Down coordinate. [meters/second] */
} hobd_imu_vel_ned2_s;


/**
 * @brief IMU orientation quaternion 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_ORIENT_QUAT1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float q1;
    //
    //
    float q2;
} hobd_imu_orient_quat1_s;


/**
 * @brief IMU orientation quaternion 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_ORIENT_QUAT2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float q3;
    //
    //
    float q4;
} hobd_imu_orient_quat2_s;


/**
 * @brief IMU rate of turn 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_RATE_OF_TURN1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float x;
    //
    //
    float y;
} hobd_imu_rate_of_turn1_s;


/**
 * @brief IMU rate of turn 2 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_RATE_OF_TURN2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float z;
} hobd_imu_rate_of_turn2_s;


/**
 * @brief IMU acceleration 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_ACCEL1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float x;
    //
    //
    float y;
} hobd_imu_accel1_s;


/**
 * @brief IMU acceleration 2 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_ACCEL2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float z;
} hobd_imu_accel2_s;


/**
 * @brief IMU magnetic field 1 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_MAGF1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float x;
    //
    //
    float y;
} hobd_imu_magf1_s;


/**
 * @brief IMU magnetic field 2 message.
 *
 * Message size (CAN frame DLC): 4 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_IMU_MAGF2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    float z;
} hobd_imu_magf2_s;


/**
 * @brief On-board diagnostics time message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_OBD_TIME
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint32_t rx_time; /*!< Local rx millisecond timestamp when the OBD data was received. [milliseconds] */
    //
    //
    uint16_t counter_1;
    //
    //
    uint16_t counter_2;
} hobd_obd_time_s;


/**
 * @brief On-board diagnostics 1 message.
 *
 * Message size (CAN frame DLC): 7 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_OBD1
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint8_t engine_on : 1;
    //
    //
    uint8_t gear : 4;
    //
    //
    uint8_t reserved : 3;
    //
    //
    uint16_t engine_rpm;
    //
    //
    uint8_t wheel_speed;
    //
    //
    uint8_t battery_volt;
    //
    //
    uint8_t tps_volt;
    //
    //
    uint8_t tps_percent;
} hobd_obd1_s;


/**
 * @brief On-board diagnostics 2 message.
 *
 * Message size (CAN frame DLC): 8 bytes
 * CAN frame ID: \ref HOBD_CAN_ID_OBD2
 * Transmit rate: TODO ms
 *
 */
typedef struct
{
    //
    //
    uint8_t ect_volt;
    //
    //
    uint8_t ect_temp;
    //
    //
    uint8_t iat_volt;
    //
    //
    uint8_t iat_temp;
    //
    //
    uint8_t map_volt;
    //
    //
    uint8_t map_pressure;
    //
    //
    uint16_t fuel_injectors;
} hobd_obd2_s;




#endif	/* HOBD_H */
