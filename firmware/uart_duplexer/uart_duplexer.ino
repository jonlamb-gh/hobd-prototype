/**
 * @file uart_duplexer.ino
 * @brief UART Duplexer.
 *
 * Arduino version: 1.6.12
 *
 * Libraries:
 * \li SoftwareSerialWithHalfDuplex
 *
 */




#include <SoftwareSerialWithHalfDuplex.h>




// *****************************************************
// static global types/macros
// *****************************************************

// ms
#define STARTUP_DELAY (2000UL)


//
#define UART_BAUDRATE (115200UL)
#define OBD_BAUDRATE (115200UL)


//
#define PIN_RX (0)
#define PIN_TX (1)


//
#define PIN_KLINE (12)




// *****************************************************
// static global data
// *****************************************************

//
static SoftwareSerialWithHalfDuplex obd_serial(
        PIN_KLINE,
        PIN_KLINE,
        false,
        false);




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
void setup( void )
{
    pinMode( PIN_RX, INPUT );
    pinMode( PIN_TX, INPUT );

    delay( STARTUP_DELAY );

    Serial.begin( UART_BAUDRATE );

    obd_serial.begin( OBD_BAUDRATE );
}


//
void loop( void )
{
    if( obd_serial.available() != 0 )
    {
        const byte rx_byte = (byte) obd_serial.read();

        Serial.write( rx_byte );
    }
}
