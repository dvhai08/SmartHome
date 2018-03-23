/**
 * Name:     Smarthome: Remote Control (MySensors)
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 3.0
 * Version:  1.0
 * Date:     2018/03/04
 *
 */



/**
 *  Pinout Remote Control V1.0
 */
#define pBTN        2 //Interruption
#define pNRF_CE     7
#define pNRF_CS    10
#define pLED_BLUE   9 //PWM
#define pATSHA204A A3



/**
 * MySensor configuration:
 */
#define MY_DEBUG                        // Enable debug prints to serial monitor
#define MY_BAUD_RATE        9600        // Serial output baud rate
#define MY_RADIO_NRF24                  // Enable and select radio type attached
#define MY_RF24_CE_PIN      pNRF_CE     // Define this to change the chip enable pin from the default
#define MY_RF24_CS_PIN      pNRF_CS     // Define this to change the chip select pin from the default

#define MS_BOARD_NAME       "Remote Control"
#define MS_SOFTWARE_VERSION "1.0"
#define MS_BTN_CHILD_ID     0
#define MS_BAT_CHILD_ID     1

#include <MySensors.h>

MyMessage msgBTN(MS_BTN_CHILD_ID, V_TRIPPED);
MyMessage msgBAT(MS_BAT_CHILD_ID, V_VAR);



/**
 * Called once at startup, usually used to initialize sensors.
 */
void setup()
{
    pinMode(pBTN,      INPUT);
    pinMode(pLED_BLUE, OUTPUT);
}



/**
 * This allows controller to re-request presentation and do re-configuring node after startup.
 */
void presentation()
{
    sendSketchInfo(MS_BOARD_NAME, MS_SOFTWARE_VERSION);
    present(MS_BTN_CHILD_ID, S_DOOR);
    present(MS_BAT_CHILD_ID, S_POWER);
}



/**
 * This will be called continuously after setup.
 */
void loop()
{
    if( digitalRead(pBTN) )
    {
        digitalWrite(pLED_BLUE, HIGH);
        while(digitalRead(pBTN));
        wait(250);

        send(msgBTN.set(true));
        send(msgBAT.set(readVcc()));
        digitalWrite(pLED_BLUE, LOW);
    }

    sleep(digitalPinToInterrupt(pBTN), CHANGE, 0);
}



/**
 * https://forum.arduino.cc/index.php?topic=356752.0
 */
long readVcc()
{
    long result;
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while( bit_is_set(ADCSRA,ADSC) );
    result = ADCL;
    result |= ADCH<<8;
    result = 1126400L / result; // Back-calculate AVcc in mV
    return result;
}