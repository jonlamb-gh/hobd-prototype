// test main.c




#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "board.h"
#include "hobd.h"

// BSP utilities
#include "timer8_drv.h"
#include "timer16_drv.h"
#include "uart_drv.h"
#include "uart_lib.h"
#include "rtc_drv.h"
#include "can_drv.h"
#include "can_lib.h"



int main( void )
{

    LED_init();
    LED_off();

    SW_init();
    SW_enable_pullup();



    while( 1 )
    {

    }



   return 0;
}
