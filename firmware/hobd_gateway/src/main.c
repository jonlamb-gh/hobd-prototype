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
#include "hobd_can.h"
#include "diagnostic_table.h"
#include "can_communication.h"
#include "hobd_communication.h"




// *****************************************************
// static global types/macros
// *****************************************************

//
#define NUM_ECU_DTABLES (3)




// *****************************************************
// static global data
// *****************************************************

//
static diagnostic_table ecu_tables[NUM_ECU_DTABLES];




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
    // zero globals
    memset( ecu_tables, 0, sizeof(ecu_tables) );

    led_init();
    led_off();

    sw_init();
    sw_enable_pullup();
}




// *****************************************************
// main
// *****************************************************
int main( void )
{
    // init
    init();





    while( 1 )
    {
        if( sw_get_state() == ON )
        {
            led_on();
            delay_ms( 10 );
        }
        else
        {
            led_off();
        }
    }

   return 0;
}
