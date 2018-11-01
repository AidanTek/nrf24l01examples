/*  NRF24L01 Transmit on Button Press Example
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

// Button Variables:
byte buttonpin = 2;
boolean buttonstate = 1;
boolean lastbuttonstate = 1;

boolean mode = 0; // Mode can be the variable to be transmitted 

void setup() {
  Serial.begin(115200);
  pinMode(buttonpin, INPUT_PULLUP);

  delay(1000);

  // Test the radio is connected properly:
  if (!radio.init(radio_id, cepin, csnpin, NRFLite::BITRATE2MBPS, radio_chan)) {
    Serial.println("Cannot communicate with radio");
    while(true); // Wait here forever.
  }

}

void loop() {
  buttonstate = digitalRead(buttonpin);

  // check if button state has changed (high to low or low to high)
  if(buttonstate != lastbuttonstate) {
    lastbuttonstate = !lastbuttonstate; // reverse logic if true

    // if the button has been pressed down (low)
    if(buttonstate == LOW) {
      mode = !mode; // reverse logic of mode

      // Let's print mode as a message:
      if(mode == 1) {
        Serial.println("Turn on the LED");
      }
      else {
        Serial.println("Turn off the LED");
      }

      // Send mode variable as a packet over radio:
      if(radio.send(send_id, &mode, sizeof(mode))) { // & must be used in front of variable
        Serial.println("...Success");
      }
      else {
        Serial.println("...Failed");
      }

    }
  }

}
