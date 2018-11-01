/*  NRF24L01 Transmit Example
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

// Libraries:
#include <SPI.h>
#include <NRFLite.h>

// Radio Variables
byte chan = 1;
byte radio_id = 1; // this radio's ID
byte send_id = 0; // the ID of the receiver radio
byte cepin = 9;
byte csnpin = 10;
byte radio_chan = 0; // please change this number to your group number

NRFLite radio; // this allows us to talk to the library in the sketch

byte counter = 0; // a variable to use to send some data

void setup() {
  Serial.begin(115200);

  // Test the radio is connected properly:
  if (!radio.init(radio_id, cepin, csnpin, NRFLite::BITRATE2MBPS, radio_chan)) {
    Serial.println("Cannot communicate with radio");
    while(true); // Wait here forever.
  }

}

void loop() {
  Serial.print("Loop count: ");
  Serial.println(counter);
  Serial.println("Sending loop count...");

  if(radio.send(send_id, &counter, sizeof(counter))) { // & must be used in front of variable
    Serial.println("...Success");
  }
  else {
    Serial.println("...Failed");
  }

  counter++; // Add 1 to the counter variable

  delay(5000); // Wait 5 seconds

}
