//******************************************************************************
//! @file $RCSfile: uart_lib.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the library of functions of:
//!             - Both UARTs
//!             - AT90CAN128/64/32
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

//_____ I N C L U D E S ________________________________________________________
#include "board.h"
#include "uart_drv.h"
#include "uart_lib.h"

//_____ D E F I N I T I O N S __________________________________________________


//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn uart_init
//!
//! UART peripheral initialization. Reset the UART, initialize the uart
//! mode, initialize the baudrate and enable the UART peripheral.
//!
//! @warning If autobaud, only one character is useful. If autobaud, one
//!          16-bit Timer is necessary.
//!
//! @param  Mode (c.f. predefined modes in "uart_drv.h" file)
//!         Baudrate (for fixed baudrate this param is not used)
//!
//! @return Baudrate Status
//!         ==0: research of timing failed
//!         ==1: baudrate performed
//!
//------------------------------------------------------------------------------
uint8_t uart_init (uint8_t mode, uint32_t baudrate)
{
    Uart_clear();       // Flush, Disable and Reset UART
    if (Uart_set_baudrate(baudrate) == 0) return 0;  //!<  c.f. macro in "uart_drv.h"
    Uart_hw_init(mode);     //!<  c.f. macro in "uart_drv.h"
    Uart_enable();          //!<  c.f. macro in "uart_drv.h"
    return (1);
}

//------------------------------------------------------------------------------
//  @fn uart_test_hit
//!
//! Check if something has been received on the UART peripheral.
//!
//! @warning none
//!
//! @param  none
//!
//! @return Baudrate Status
//!         ==0: Nothing has been received
//!         ==1: A character has been received
//!
//------------------------------------------------------------------------------
uint8_t uart_test_hit (void)
{
    return (Uart_rx_ready());
}

//------------------------------------------------------------------------------
//  @fn uart_putchar
//!
//! Send a character on the UART peripheral.
//!
//! @warning none
//!
//! @param  character to send
//!
//! @return character sent
//!
//------------------------------------------------------------------------------
uint8_t uart_putchar (uint8_t ch)
{
    while(!Uart_tx_ready());
    Uart_set_tx_busy();     // Set Busy flag before sending (always)
    Uart_send_byte(ch);
    return (ch);
}

//------------------------------------------------------------------------------
//  @fn uart_getchar
//!
//! Get a character from the UART peripheral.
//!
//! @warning none
//!
//! @param  none
//!
//! @return read (received) character on the UART
//!
//------------------------------------------------------------------------------
uint8_t uart_getchar (void)
{
    uint8_t ch;

    while(!Uart_rx_ready());
    ch = Uart_get_byte();
    Uart_ack_rx_byte();
    return ch;
}

//------------------------------------------------------------------------------
//  @fn uart_put_string
//!
//! Put a data-string on TX UART. The data-string is send up to null
//! character is found.
//!
//! @warning "uart_init()" must be performed before
//!
//! @param Pointer on uint8_t data-string
//!
//! @return (none)
//!
//------------------------------------------------------------------------------
#ifndef REDUCED_UART_LIB
void uart_put_string (uint8_t *data_string)
    {
    while(*data_string) uart_putchar (*data_string++);
    }
#endif  // REDUCED_UART_LIB
