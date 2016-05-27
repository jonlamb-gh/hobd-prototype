/**
 * @file board.h
 * @brief TODO.
 *
 * @todo board details.
 *
 * UART0 - pins ? wired to DB9
 * UART1 - ...
 *
 */




#ifndef BOARD_H
#define BOARD_H




#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <stdint.h>




//
typedef uint8_t BOOL;


//
#define OFF (0)
#define ON  (1)


//
#define MAX(a, b) ( (a)>(b) ? (a) : (b) )
#define MIN(a, b) ( (a)<(b) ? (a) : (b) )


//
#define enable_interrupt() { asm ("sei"::) ; }


//
#define disable_interrupt() { asm ("cli"::) ; }


//
#define hard_reset() { WDTCR |= 1<<WDE;  while(1); }


// chip
#ifndef __AVR_AT90CAN128__
#define __AVR_AT90CAN128__ 1
#endif


// 16 MHz
#define FOSC (16000)
#define F_CPU (FOSC * 1000)


// uart config
#define USE_TIMER16 BOTH_TIMER16
#define USE_UART BOTH_UART
#define UART_BAUDRATE VARIABLE_UART_BAUDRATE


// RTC config
#define USE_TIMER8 TIMER8_2
#define RTC_TIMER (2)
#define RTC_CLOCK (0)


// CAN
#define CAN_BAUDRATE (500)


//
#define HOBD_UART UART_1
#define DEBUG_UART UART_0


//
#define DEBUG_BAUDDRATE (57600UL)
#define HOBD_BAUDRATE (10400UL)


//
#define LED_PORT_IN PINE
#define LED_PORT_DDR DDRE
#define LED_PORT_OUT PORTE
#define LED_STATUS_0 (4)


#define SW_PORT_IN PINE
#define SW_PORT_DDR DDRE
#define SW_PORT_OUT PORTE
#define SW_STATUS_0 (5)


#define led_init() (LED_PORT_DDR |= (1 << LED_STATUS_0))
#define led_on() (LED_PORT_OUT &= 0b11101111)
#define led_off() (LED_PORT_OUT |= 0b00010000)
#define led_toggle() (LED_PORT_OUT ^= (1 << LED_STATUS_0))


#define sw_init() (SW_PORT_DDR &= ~(1 << SW_STATUS_0))
#define sw_enable_pullup() (SW_PORT_OUT |= (1 << SW_STATUS_0))
#define sw_get_state() (!(SW_PORT_IN & (1 << SW_STATUS_0)))




#endif // BOARD_H
