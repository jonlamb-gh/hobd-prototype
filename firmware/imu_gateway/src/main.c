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
#include "time.h"




#ifdef BUILD_TYPE_DEBUG
#warning "BUILD_TYPE_DEBUG ON"
#endif




// *****************************************************
// static global types/macros
// *****************************************************

//
#define NODE_ID (0x06)


//
#define HARDWARE_VERSION (1)


//
#define FIRMWARE_VERSION (1)




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

    // init enclosure switch pin
    sw1_init();
    sw1_enable_pullup();

    // init LED pin
    led_init();
    led_off();

    // enable watchdog, reset after 120 ms
    wdt_enable( WDTO_120MS );

    // reset watchdog
    wdt_reset();

    //
    rtc_int_init();

    //
    enable_interrupt();

    //
    Uart_select( GPS_UART );
    uart_init( CONF_8BIT_NOPAR_1STOP, GPS_BAUDRATE );

    //
#ifdef BUILD_TYPE_DEBUG
	Uart_select( DEBUG_UART );
    uart_init( CONF_8BIT_NOPAR_1STOP, DEBUG_BAUDDRATE );
#endif

    time_sleep_ms( 5 );

    DEBUG_PUTS( "init : pass\r\n" );
}




// *****************************************************
// main
// *****************************************************
int main( void )
{
    //
    init();

    //
    while( 1 )
    {
        // reset watchdog
        wdt_reset();

        // turn on the on-board LED if either switch is closed
        if( (sw0_get_state() == ON) || (sw1_get_state() == ON) )
        {
            led_on();
        }
        else
        {
            led_off();
        }
    }

   return 0;
}
