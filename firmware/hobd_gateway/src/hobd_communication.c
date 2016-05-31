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


//
uint8_t hobdcomm_is_valid_packet( const uint8_t * const buffer, const uint8_t len )
{
    uint8_t packet_type = HOBD_PACKET_TYPE_INVALID;

    // cast header
    const hobd_packet_header * const header =
            (hobd_packet_header*) buffer;

    // min size = 3 byte header plus 1 byte checksum
    const uint8_t min_len = (uint8_t) (sizeof(*header) + 1);

    //
    if( (len >= min_len) && (header->size >= min_len) && (header->size <= len) )
    {
        if( header->type != HOBD_PACKET_TYPE_INVALID )
        {
            const uint8_t packet_checksum = buffer[ header->size - 1 ];

            const uint8_t real_checksum = hobdcomm_checksum( buffer, header->size );

            //
            if( packet_checksum == real_checksum )
            {
                packet_type = header->type;
            }
        }
    }

    return packet_type;
}
