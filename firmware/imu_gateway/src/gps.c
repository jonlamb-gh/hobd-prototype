/**
 * @file gps.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <inttypes.h>

#include "board.h"
#include "uart_lib.h"
#include "uart_drv.h"
#include "ring_buffer.h"
#include "gps.h"




// *****************************************************
// static global types/macros
// *****************************************************

// GPS is on UART1
#define UART_RX_INTERRUPT USART1_RX_vect
#define UART_STATUS UCSR1A
#define UART_CONTROL UCSR1B
#define UART_DATA UDR1


//
#define gps_uart_enable() (UART_CONTROL |= (_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))


//
#define gps_uart_disable() (UART_CONTROL &= ~(_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))




// *****************************************************
// static global data
// *****************************************************

//
static volatile ring_buffer_s rx_buffer;




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static uint8_t hw_init( void )
{
    uint8_t ret = 0;

    Uart_select( GPS_UART );

    Uart_clear();

    if( Uart_set_baudrate( GPS_BAUDRATE ) == 0 )
    {
        ret = 1;
    }
    else
    {
        Uart_hw_init( CONF_8BIT_NOPAR_1STOP );
        gps_uart_enable();
    }

    return ret;
}


//
ISR( UART_RX_INTERRUPT )
{
    // read UART status register and UART data register
    const uint8_t status  = UART_STATUS;
    const uint8_t data = UART_DATA;

    // read error status
    rx_buffer.error = (status & (_BV(FE1) | _BV(DOR1)) );

    // push data into the rx buffer, error is updated with return status
    (void) ring_buffer_putc(
            data,
            &rx_buffer );
}




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t gps_init( void )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    ret = hw_init();

    return ret;
}
