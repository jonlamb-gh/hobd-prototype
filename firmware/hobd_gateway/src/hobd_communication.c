/**
 * @file hobd_communication.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "board.h"
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


//
uint8_t hobdcomm_send( void )
{
    return 0;
}


//
uint8_t hobdcomm_recv(
        uint8_t * const buffer,
        const uint8_t max_len )
{
    uint8_t bytes_read = 0;

    //
    Uart_select( HOBD_UART );

    //
    while( uart_test_hit() != 0 )
    {
        const uint8_t rx_byte = (uint8_t) uart_getchar();

        if( (bytes_read < HOBD_PACKET_SIZE_MAX) && (bytes_read < max_len) )
        {
            buffer[ bytes_read ] = rx_byte;

            bytes_read += 1;
        }
    }


    return bytes_read;
}


//
uint8_t hobdcomm_checksum( const uint8_t * const buffer, const uint8_t len )
{
    uint16_t cs =0;
    uint8_t idx = 0;

    for( idx = 0; idx < len; idx += 1 )
    {
        cs += (uint16_t) buffer[ idx ];
    }

    if( cs > 0x0100 )
    {
        cs = (0x0100 - (cs & 0x00FF));
    }
    else
    {
        cs = (0x0100 - cs);
    }


    return (uint8_t) cs;
}
