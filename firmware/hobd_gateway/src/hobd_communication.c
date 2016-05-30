/**
 * @file hobd_communication.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "uart_lib.h"
#include "hobd.h"
#include "hobd_communication.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t hobdcomm_init( void )
{
    uint8_t ret = 0;

    // select UART
	Uart_select( HOBD_UART );

    //
    if( uart_init( CONF_8BIT_NOPAR_1STOP, HOBD_BAUDRATE ) == 0 )
    {
        ret = 1;
    }

    return ret;
}
