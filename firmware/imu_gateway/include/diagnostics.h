/**
 * @file diagnostics.h
 * @brief TODO.
 *
 */




#ifndef DIAGNOSTICS_H
#define	DIAGNOSTICS_H




#include <inttypes.h>



//
#define DIAGNOSTICS_CLEAR_SET_NONE (0x0000)


// 60,000 ms == 60 seconds
#define DIAGNOSTICS_WARN_SET_CLEAR_INTERVAL (60000UL)


// error state
// ms
#define DIAGNOSTICS_SLOW_BLINK_INTERVAL (2000UL)


// GPS lock
// ms
#define DIAGNOSTICS_MED_BLINK_INTERVAL (500UL)


// IMU lock
// ms
#define DIAGNOSTICS_FAST_BLINK_INTERVAL (100UL)




//
void diagnostics_init( void );


//
void diagnostics_set_state(
        const uint8_t state );


//
uint8_t diagnostics_get_state( void );


//
void diagnostics_set_warn(
        const uint16_t warn );


//
uint16_t diagnostics_get_warn( void );


//
void diagnostics_clear_warn(
        const uint16_t warn );


//
void diagnostics_set_error(
        const uint16_t error );


//
uint16_t diagnostics_get_error( void );


//
void diagnostics_clear_error(
        const uint16_t error );


//
void diagnostics_set_warn_timeout_bits( void );


//
void diagnostics_update( void );




#endif	/* DIAGNOSTICS_H */
