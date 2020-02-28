/*  NRF24L01 Transceiver example
 *  This example uses Dave Parson's NRFLite Library
 *  
 *  Example by Aidan Taylor 2020 for Engineering Science
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
byte my_id = 1; // MAKE SURE YOU CHANGE THIS ON BOTH ARDUINOS - each Arduino should send to the other's ID
byte send_id = 0; // MAKE SURE YOU CHANGE THIS ON BOTH ARDUINOS - each Arduino should send to the other's ID
byte cepin = 9;
byte csnpin = 10;
byte radio_chan = 0; // please change this number to your group number

NRFLite radio;

// Button Variables:
byte buttonpin = 2;
boolean buttonstate = 1;
boolean lastbuttonstate = 1;

// Some variables for the radio:
byte randomfifty = 0; // Random number storage
byte data_tx = 0; // this variable can be used to send a data packet
byte data_rx = 0; // this variable will be updated with the incoming message

void setup() {
  Serial.begin(115200);
  pinMode(buttonpin, INPUT_PULLUP);

  // Check the radio is connected correctly:
  if (!radio.init(my_id, cepin, csnpin, NRFLite::BITRATE2MBPS, radio_chan)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }

}

void loop() {
  // For every iteration of loop, we check for a button press and check for an incoming radio packet:

  // Let's check for a button press first:
  buttonstate = digitalRead(buttonpin);

  // check if button state has changed (high to low or low to high)
  if(buttonstate != lastbuttonstate) {
    lastbuttonstate = !lastbuttonstate; // reverse logic if true

    // if the button has been pressed down (low)
    if(buttonstate == LOW) {
      delay(20); // A short delay to avoid 'bouncing'
      Serial.println("The button was pressed");

      // Send 255 as a radio packet:
      data_tx = 255;

      // Send data_tx variable as a packet over radio:
      if(radio.send(send_id, &data_tx, sizeof(data_tx))) { // & must be used in front of variable
        Serial.println("...Message sent");
      }
      else {
        Serial.println("...Failed to send message");
      }

    }
  }

  // Next we check for an incoming radio packet:
  while(radio.hasData()) {
    radio.readData(&data_rx); // & must be placed in front of the variable

    // Check data_rx to decide what to do...
    if(data_rx == 255) { // Button press on other Arduino, generate a random number to send
      Serial.println("Radio packet received, someone pressed a button...");
      randomfifty = random(50)+1;
      delay(500);
      Serial.print("Generated the random number: "); Serial.println(randomfifty);
      delay(500);
      // Send the random number
      data_tx = randomfifty;
      if(radio.send(send_id, &data_tx, sizeof(data_tx))) { // & must be used in front of variable
        Serial.println("...Sent random number back");
      }
      else {
        Serial.println("...Failed to send the message");
      }

    }
     // If the contents of the received packet is not 255 then it is a random number coming back:
    else {
      String msg = "Random number received: ";
      msg += data_rx;

      Serial.println("A message was received...");
      delay(500);

      Serial.println(msg); // print the message
    }
  }  
}
