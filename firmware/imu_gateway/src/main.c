/**
 * @file main.c
 * @brief Main.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

//
#include "board.h"

// BSP utilities
#include "timer8_drv.h"
#include "timer16_drv.h"
#include "uart_drv.h"
#include "uart_lib.h"
#include "rtc_drv.h"
#include "can_drv.h"
#include "can_lib.h"

//
#include "hobd.h"
#include "time.h"
#include "canbus.h"
#include "diagnostics.h"
#include "gps.h"
#include "imu.h"




#ifdef BUILD_TYPE_DEBUG
#warning "BUILD_TYPE_DEBUG ON"
#endif




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************




// *****************************************************
// static declarations
// *****************************************************

//
static void init( void );




// *****************************************************
// static definitions
// *****************************************************

//
static void init( void )
{
    // disable watchdog
    wdt_disable();

    // init on-board switch pin
    sw0_init();
    sw0_enable_pullup();

    // init LED pin
    led_init();
    led_off();

    // enable watchdog, reset after 120 ms
    wdt_enable( WDTO_120MS );

    // reset watchdog
    wdt_reset();

    //
    rtc_int_init();

    // enable interrupts
    enable_interrupt();

    //
    const uint8_t can_status = canbus_init();

    //
    diagnostics_init();

    // init GPS UART/module
    const uint8_t gps_status = gps_init();

#ifdef BUILD_TYPE_DEBUG
    // debug UART is used instead of IMU
    Uart_select( DEBUG_UART );
    uart_init( CONF_8BIT_NOPAR_1STOP, DEBUG_BAUDRATE );

    const uint8_t imu_status = 0;
#else
    // init IMU UART/module
    const uint8_t imu_status = imu_init();
#endif

    // enable interrupts
    enable_interrupt();

    // reset watchdog
    wdt_reset();

    //
    if( can_status != 0 )
    {
        diagnostics_set_error( HOBD_HEARTBEAT_ERROR_CANBUS );
        DEBUG_PUTS( "init : canbus_init fail\n" );
    }

    //
    if( gps_status != 0 )
    {
        diagnostics_set_error( HOBD_HEARTBEAT_ERROR_GPSBUS );
        DEBUG_PUTS( "init : gps_init fail\n" );
    }

    //
    if( imu_status != 0 )
    {
        diagnostics_set_error( HOBD_HEARTBEAT_ERROR_IMUBUS );
        DEBUG_PUTS( "init : imu_init fail\n" );
    }

    // set the warn clear set, bits that are cleared on a timer
    diagnostics_set_warn_timeout_bits(
            HOBD_HEARTBEAT_WARN_CANBUS |
            HOBD_HEARTBEAT_WARN_IMUBUS |
            HOBD_HEARTBEAT_WARN_GPSBUS );

    // set initial warnings
    diagnostics_set_warn( HOBD_HEARTBEAT_WARN_NO_GPS_FIX );
    diagnostics_set_warn( HOBD_HEARTBEAT_WARN_NO_IMU_FIX );

    time_sleep_ms( 5 );

    DEBUG_PUTS( "init : pass\n" );

    // reset watchdog
    wdt_reset();

    //
    diagnostics_update();
}




// *****************************************************
// main
// *****************************************************
int main( void )
{
    //
    init();

    // reset watchdog
    wdt_reset();

    //
    diagnostics_set_state( HOBD_HEARTBEAT_STATE_OK );

    //
    while( 1 )
    {
        // reset watchdog
        wdt_reset();

        // process any incoming GPS data, and potentially publish ready CAN frames
        const uint8_t gps_status = gps_update();

        // TODO - better error/warn handling
        if( gps_status != 0 )
        {
            diagnostics_set_warn( HOBD_HEARTBEAT_WARN_GPSBUS );
        }

        // process any incoming IMU data, and potentially publish ready CAN frames
        const uint8_t imu_status = imu_update();

        // TODO - better error/warn handling
        if( imu_status != 0 )
        {
            diagnostics_set_warn( HOBD_HEARTBEAT_WARN_IMUBUS );
        }

        //
        diagnostics_update();
    }

   return 0;
}
