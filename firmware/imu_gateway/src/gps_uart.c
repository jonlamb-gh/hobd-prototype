/**
 * @file gps_uart.c
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
#include "gps_uart.h"




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
#define RX_BUFFER_SIZE (128)


//
#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)




//
typedef struct
{
    //
    //
    uint8_t head;
    //
    //
    uint8_t tail;
    //
    //
    uint8_t error;
    //
    //
    uint8_t buffer[ RX_BUFFER_SIZE ];
} ring_buffer_s;




// *****************************************************
// static global data
// *****************************************************

//
static ring_buffer_s rx_buffer;




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static void ring_buffer_init(
        ring_buffer_s * const rb )
{
    if( rb != NULL )
    {
        rb->head = 0;
        rb->tail = 0;
        rb->error = 0;
    }
}


//
static uint8_t uart_hw_init( void )
{
    uint8_t ret = 0;

    Uart_select( GPS_UART );

    Uart_clear();

    if( Uart_set_baudrate( GPS_BAUDRATE ) == 0 )
    {
        ret = 1;
    }

    if( ret == 0 )
    {
        Uart_hw_init( CONF_8BIT_NOPAR_1STOP );
    }

    if( ret == 0 )
    {
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
    uint8_t error = (status & (_BV(FE1) | _BV(DOR1)) );

    // calculate buffer index
    const uint16_t new_head =  (uint16_t) (rx_buffer.head + 1) & RX_BUFFER_MASK;

    if( new_head == rx_buffer.tail )
    {
#warning "TODO - handle overflow - see 'https://github.com/andygock/avr-uart' for reference"
        // receive buffer overflow error
//        error = UART_BUFFER_OVERFLOW >> 8;
    }
    else
    {
        // store new index
        rx_buffer.head = new_head;

        // store received data in buffer
        rx_buffer.buffer[ new_head ] = data;
    }

    rx_buffer.error = error;
}




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t gps_uart_init( void )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    ret = uart_hw_init();

    return ret;
}
