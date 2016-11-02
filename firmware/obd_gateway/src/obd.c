/**
 * @file obd.c
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
#include "uart_lib.h"
#include "uart_drv.h"
#include "hobd.h"
#include "ring_buffer.h"
#include "time.h"
#include "canbus.h"
#include "diagnostics.h"
#include "obd.h"




// *****************************************************
// static global types/macros
// *****************************************************

// UART bridge is on UART1
#define UART_RX_INTERRUPT USART1_RX_vect
#define UART_UCSRA UCSR1A
#define UART_UCSRB UCSR1B
#define UART_UCSRC UCSR1C
#define UART_DATA UDR1


//
#define obd_uart_enable() (UART_UCSRB |= (_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))


//
#define obd_uart_disable() (UART_UCSRB &= ~(_BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1)))




// *****************************************************
// static global data
// *****************************************************

// UART rx ring buffer
static volatile ring_buffer_s rx_buffer;


// GPS message/data state
static obd_data_s obd_data;


// last rx time
static uint32_t last_rx_time = 0;




// *****************************************************
// static declarations
// *****************************************************

//
static void hw_init( void );


//
static void update_rx_timeout(
        const uint32_t * const now );




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
    rx_buffer.error = (status & (_BV(FE1) | _BV(DOR1)) );

    // push data into the rx buffer, error is updated with return status
    (void) ring_buffer_putc(
            data,
            &rx_buffer );
}


//
static void hw_init( void )
{
    Uart_select( OBD_UART );

    Uart_clear();

    Uart_set_ubrr( OBD_BAUDRATE );

    Uart_hw_init( CONF_8BIT_NOPAR_1STOP );

    obd_uart_enable();
}


//
static void update_rx_timeout(
        const uint32_t * const now )
{
    const uint16_t warn = diagnostics_get_warn();

    // if warn not set
    if( (warn & HOBD_HEARTBEAT_WARN_NO_OBD_ECU) == 0 )
    {
        // get time since last rx update
        const uint32_t delta = time_get_delta(
                &last_rx_time,
                now );

        // set warning if interval met/exceeded
        if( delta >= OBD_RX_WARN_TIMEOUT )
        {
            diagnostics_set_warn( HOBD_HEARTBEAT_WARN_NO_OBD_ECU );
        }
    }
}




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t obd_init( void )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    hw_init();

    // clear all ready groups
    obd_clear_all_group_ready();

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    return ret;
}


//
void obd_disable( void )
{
    // disable UART
    obd_uart_disable();

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );
}


//
void obd_enable( void )
{
    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    // enable UART
    obd_uart_enable();
}


//
void obd_set_group_ready(
        const uint16_t group )
{
    obd_data.ready_groups |= group;
}


//
void obd_clear_group_ready(
        const uint16_t group )
{
    obd_data.ready_groups &= ~group;
}


//
void obd_clear_all_group_ready( void )
{
    obd_data.ready_groups = OBD_GROUP_NONE_READY;
}


//
uint8_t obd_is_group_ready(
        const uint16_t group )
{
    return ((obd_data.ready_groups & group) == 0) ? 0 : 1;
}


//
uint8_t obd_update( void )
{
    uint8_t ret = 0;

    // TODO

    // get current time
    const uint32_t now = time_get_ms();

    // update rx timeout status/warning
    update_rx_timeout( &now );

    return ret;
}
