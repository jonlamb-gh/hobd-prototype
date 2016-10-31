/**
 * @file imu.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <math.h>

#include "board.h"
#include "xbusmessage.h"
#include "xbusparser.h"
#include "uart_lib.h"
#include "uart_drv.h"
#include "hobd.h"
#include "ring_buffer.h"
#include "time.h"
#include "canbus.h"
#include "diagnostics.h"
#include "imu.h"




// *****************************************************
// static global types/macros
// *****************************************************

//
#define XBUS_BUFFER_SIZE (512)

// IMU is on UART0
#define UART_RX_INTERRUPT USART0_RX_vect
#define UART_UCSRA UCSR0A
#define UART_UCSRB UCSR0B
#define UART_UCSRC UCSR0C
#define UART_DATA UDR0


//
#define imu_uart_enable() (UART_UCSRB |= (_BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0)))


//
#define imu_uart_disable() (UART_UCSRB &= ~(_BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0)))




// *****************************************************
// static global data
// *****************************************************

// UART rx ring buffer
static volatile ring_buffer_s rx_buffer;


// Xbus parser state
static struct XbusParser xbus_parser;


// Xbux static buffer
static uint8_t xbus_buffer[ XBUS_BUFFER_SIZE ];


// IMU message/data state
static imu_data_s imu_data;




// *****************************************************
// static declarations
// *****************************************************

//
static void hw_init( void );




// *****************************************************
// static definitions
// *****************************************************

//
ISR( UART_RX_INTERRUPT )
{
    // read UART status register and UART data register
    const uint8_t status  = UART_UCSRA;
    const uint8_t data = UART_DATA;

    // read error status
    rx_buffer.error = (status & (_BV(FE0) | _BV(DOR0)) );

    // push data into the rx buffer, error is updated with return status
    (void) ring_buffer_putc(
            data,
            &rx_buffer );
}


//
static void hw_init( void )
{
    Uart_select( IMU_UART );

    Uart_clear();

    Uart_set_ubrr( IMU_BAUDRATE );

    Uart_hw_init( CONF_8BIT_NOPAR_1STOP );

    imu_uart_enable();
}


//
static uint8_t process_buffer( void )
{
    uint8_t ret = 0;

    const uint16_t data = ring_buffer_getc( &rx_buffer );

    if( data != RING_BUFFER_NO_DATA )
    {
        XbusParser_parseByte( &xbus_parser, (uint8_t) (data & 0xFF) );
    }

    return ret;
}


//
static void *xbus_alloc_cb( size_t size )
{
    if( size > sizeof(xbus_buffer) )
    {
        diagnostics_set_error( HOBD_HEARTBEAT_ERROR_IMU_RX_OVERFLOW );
    }

    return (void*) &xbus_buffer[0];
}


//
static void xbus_free_cb( void const * buffer )
{
    // do nothing to our static buffer
}


//
static void handle_message_cb( struct XbusMessage const * message )
{
#warning "TODO - message handler - may need to add support for double types or configure for floats"

    if( message->length > (uint16_t) sizeof(xbus_buffer) )
    {
        diagnostics_set_error( HOBD_HEARTBEAT_ERROR_IMU_RX_OVERFLOW );
    }
    else
    {
        // TODO
    }



    // TODO
//    float ori[4];
//    if (XbusMessage_getDataItem(ori, XDI_Quaternion, message))
//    {
//        pc.printf(" Orientation: (% .3f, % .3f, % .3f, % .3f)", ori[0], ori[1],
//                ori[2], ori[3]);
//    }
//    float acc[3];
//    if (XbusMessage_getDataItem(acc, XDI_Acceleration, message))
//    {
//        pc.printf(" Acceleration: (% .3f, % .3f, % .3f)", acc[0], acc[1], acc[2]);
//    }
//    float gyr[3];
//    if (XbusMessage_getDataItem(gyr, XDI_RateOfTurn, message))
//    {
//        pc.printf(" Rate Of Turn: (% .3f, % .3f, % .3f)", gyr[0], gyr[1], gyr[2]);
//    }
}




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t imu_init( void )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    const struct XbusParserCallback xbus_callbacks =
    {
        .allocateBuffer = &xbus_alloc_cb,
        .deallocateBuffer = &xbus_free_cb,
        .handleMessage = &handle_message_cb
    };

    (void) XbusParser_init(
            (void*) &xbus_parser,
            &xbus_callbacks );

    hw_init();

    // clear all ready groups
//    imu_clear_all_group_ready();

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    return ret;
}


//
void imu_disable( void )
{
    // disable UART
    imu_uart_disable();

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );
}


//
void imu_enable( void )
{
    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    // enable UART
    imu_uart_enable();
}


//
uint8_t imu_update( void )
{
    uint8_t ret = 0;

    // process any available data in the rx buffer, callbacks are called from
    // this context
    ret = process_buffer();

    return ret;
}
