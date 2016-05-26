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
#define HOBD_CAN_HEARTBEAT_STATE_INIT (0x00)


//
#define HOBD_CAN_HEARTBEAT_STATE_OK (0x01)




// TODO - add hobd rx error counter ? or update timestamp ?
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
    uint8_t state : 4;
    //
    //
    uint8_t reserved_0 : 4;
    //
    //
    uint8_t reserved_1;
    //
    //
    uint16_t error_register;
    //
    //
    uint16_t warning_register;
} hobd_can_heartbeat_msg;




#endif	/* HOBD_CAN_H */
