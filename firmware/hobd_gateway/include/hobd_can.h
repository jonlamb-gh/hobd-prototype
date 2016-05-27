/**
 * @file hobd_can.h
 * @brief Honda OBD Gateway CAN Protocol.
 *
 */




#ifndef HOBD_CAN_H
#define	HOBD_CAN_H




#include <stdint.h>




//
#define HOBD_CAN_ID_HEARTBEAT_BASE (0x100)


// ms
#define HOBD_CAN_TX_INTERVAL_HEARTBEAT (50)


//
#define HOBD_CAN_ID_COMMAND (0x200)


//
#define HOBD_CAN_ID_RESPONSE (0x201)


//
#define HOBD_CAN_ID_DTABLE_REGISTER (0x250)


//
#define HOBD_CAN_HEARTBEAT_STATE_INIT (0x00)


//
#define HOBD_CAN_HEARTBEAT_STATE_OK (0x01)




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
} hobd_can_heartbeat_msg;


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
} hobd_can_command_msg;


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
} hobd_can_response_msg;


//
typedef struct
{
    //
    //
    uint8_t table_id;
    //
    //
    uint8_t offset;
    //
    //
    uint32_t last_update;
    //
    //
    uint16_t value;
} hobd_can_dtable_register_msg;




#endif	/* HOBD_CAN_H */
