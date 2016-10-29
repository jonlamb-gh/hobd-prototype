/**
 * @file imu.h
 * @brief TODO.
 *
 */




#ifndef IMU_H
#define	IMU_H




#include <inttypes.h>

#include "hobd.h"




//
uint8_t imu_init( void );


//
void imu_disable( void );


//
void imu_enable( void );


//
uint8_t imu_update( void );




#endif	/* IMU_H */
