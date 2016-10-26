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
#include "libsbp/sbp.h"
#include "libsbp/system.h"
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


//
static sbp_state_t sbp_state;


//
static sbp_msg_callbacks_node_t heartbeat_callback_node;




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

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
static uint32_t sbp_read_function( uint8_t *buff, uint32_t buff_len, void *context )
{
    uint32_t idx = 0;

    for( idx = 0; idx < buff_len; idx += 1 )
    {
        const uint16_t data = ring_buffer_getc( &rx_buffer );

        if( data == RING_BUFFER_NO_DATA )
        {
            break;
        }
        else
        {
            buff[ idx ] = (uint8_t) (data & 0xFF);
        }
    }

    return idx;
}


//
static void heartbeat_callback( uint16_t sender_id, uint8_t msg_len, uint8_t msg[], void *context )
{
    const msg_heartbeat_t * const heartbeat = (const msg_heartbeat_t*) msg;

    DEBUG_PRINTF( "gps_heartbeat : flags 0x%X\r\n", heartbeat->flags );
}




// *****************************************************
// public definitions
// *****************************************************

//
uint8_t gps_init( void )
{
    uint8_t ret = 0;

    ring_buffer_init( &rx_buffer );

    sbp_state_init( &sbp_state );

#warning "TODO - check SBP cb status"

    const int8_t sbp_status = sbp_register_callback(
            &sbp_state,
            SBP_MSG_HEARTBEAT,
            &heartbeat_callback,
            NULL,
            &heartbeat_callback_node );

    ret = hw_init();

    return ret;
}


//
void gps_disable( void )
{
    // disable UART
    gps_uart_disable();

    // flush rx buffer
    ring_buffer_flush( &rx_buffer );
}


//
void gps_enable( void )
{
    // flush rx buffer
    ring_buffer_flush( &rx_buffer );

    // enable UART
    gps_uart_enable();
}


//
uint8_t gps_update( void )
{
    uint8_t ret = 0;

#warning "TODO - check SBP read status"

    // process any available data in the rx buffer, callbacks are called from
    // this context
    const int8_t sbp_status = sbp_process(
            &sbp_state,
            &sbp_read_function );

    if( sbp_status != SBP_OK )
    {
        DEBUG_PRINTF( "gps_enable : sbp_process %d\r\n", sbp_status );
    }

    return ret;
}
