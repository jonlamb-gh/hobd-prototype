//******************************************************************************
//! @file $RCSfile: uart_lib.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        library of functions of:
//!             - Both UARTs
//!             - for AT90CAN128/64/32.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _UART_LIB_H_
#define _UART_LIB_H_

//_____ I N C L U D E S ________________________________________________________
#include "board.h"
#include "uart_drv.h"

//_____ D E F I N I T I O N S __________________________________________________
#define DATA_BUF_LEN   (12)       // Used in uart_mini_printf
                                // If the Max number is an uint32_t = 0xFFFFFFFF
                                // (8 digits) or = 4294967295 (10 digits) so
                                // 12 is enough if sign with (and) "0" prefixes
                                // are used.

//_____ D E C L A R A T I O N S ________________________________________________

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
extern uint8_t uart_init (uint8_t mode, uint32_t baudrate);

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
extern uint8_t uart_test_hit (void);

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
extern uint8_t uart_putchar (uint8_t ch);

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
extern uint8_t uart_getchar (void);

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
void uart_put_string (uint8_t *data_string);


//______________________________________________________________________________

#endif  // _UART_LIB_H_
