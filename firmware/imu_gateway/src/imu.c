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


// SBP state
//static sbp_state_t sbp_state;


// GPS message/data state
//static gps_data_s gps_data;




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
    rx_buffer.error = (status & (_BV(FE1) | _BV(DOR1)) );

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




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t imu_init( void )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    //
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

    // TESTING
#warning "TODO - imu_update() logic"
    ring_buffer_flush( &rx_buffer );

    return ret;
}
