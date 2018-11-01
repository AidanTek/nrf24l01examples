/*  NRF24L01 Receive Example
 *  This example uses Dave Parson's NRFLite Library
 *  
 *  Example by Aidan Taylor 2018 for Engineering Science
 *  Cardiff Metropolitan University
 */

/*
Connections:

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND
*/

#include <SPI.h>
#include <NRFLite.h>

// Radio Variables
byte radio_id = 0;
byte cepin = 9;
byte csnpin = 10;
byte radio_chan = 0; // please change this number to your group number

NRFLite radio;

boolean data_rx = 0; // this variable will be updated with the incoming message

void setup() {
  Serial.begin(115200);

  // Check the radio is connected correctly:
  if (!radio.init(radio_id, cepin, csnpin, NRFLite::BITRATE2MBPS, radio_chan)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }

}

void loop() {
  while(radio.hasData()) {
    radio.readData(&data_rx); // & must be placed in front of the variable

    String msg = "Message Received: ";
    msg += data_rx; // += will append the variable to the string msg

    Serial.println(msg); // print the message
  }

}
