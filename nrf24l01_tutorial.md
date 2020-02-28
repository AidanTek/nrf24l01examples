# NRF24L01 Radio Transceiver

## Introduction

The nRF24L01 is a module which includes a radio chip and antenna. It operates at frequencies around 2.4GHz. The modules can be set to one of 125 (0 - 124) channels, with each channel being seperated by 1MHz and multiple modules can communicate on a single channel. Each module on a channel can be given a unique ID, so it is easy to create a network of devices that can talk to each other over a short range.

The module is very useful for short range radio communication, small data packets can be sent between multiple data packets for fast wireless communication, great for remote control devices like toys, home automation and robotics.

## Library

* There are a couple of Arduino libraries available for the nRF24L01, I find "NRFLite" by Dave Parson is very good. NRFLite can be installed via the Library manager within Arduino.
* This library has a lot of good examples that I recommend you check out to see the capabilities of the module, there is also a YouTube tutorial: https://www.youtube.com/watch?v=tWEgvS7Sj-8&t=223s

## Connections

* I have provided a breakout circuit board which makes the NRF24L01 easy to connect to a breadboard, it needs to be soldered. Pay attention to the image:

![nrf24l01solderA](https://electricnoodlebox.files.wordpress.com/2018/11/nrfsolder1.jpg)
![nrf24lo1solderB](https://electricnoodlebox.files.wordpress.com/2018/11/nrfsolder2.jpg)

* **WARNING This is a 3v3 device, not 5V!! Connecting to 5V will permanently damage the module, use the 3v3 pin on the Arduino to power the module**
* Connect the module like so:

| nRF24L01 | Arduino Uno |
| --- | --- |
| 3v3 | 3v3 |
| GND | GND |
| CE | 9 |
| CSN | 10 |
| MOSI | 11 |
| MISO | 12 |
| SCK | 13 |
| IRQ | Not required |

![nrf24l01wired1](https://electricnoodlebox.files.wordpress.com/2018/11/nrfarduino2.jpg)
![nrf24lo1wired2](https://electricnoodlebox.files.wordpress.com/2018/11/nrfarduino1.jpg)


## Examples
	
### Basic Transmit

This example shows how to send a basic message with the nRF24L01, it needs to be paired with an Arduino running the Basic Receive example which follows below. 

In this sketch there is a variable called counter which is a byte, an 8-bit number (has a range of 0 to 255). Every iteration of 'loop' will add 1 to counter, and will send it as a data packet to a receiving radio.

* please set the radio channel in the example sketch to your group number, otherwise radio traffic is going to cause problems!
* Each transciever module on a channel should have a unique ID, which should just be a number between 0 - 255 (one byte). You need to set the transmit radio's ID for this Arduino, and also the ID of the receiver radio you want to send to.

```cpp
/*  nRF24L01 Transmit Example
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
```

### Basic Receive

This example shows how to receive a basic message with the nRF24L01, and should be paired with an Arduino running the Basic Transmit example provided above.

The 'loop' of this sketch has some code that constantly checks for an incoming data packet on the set radio channel. A receive message is stored in the data_rx variable. data_rx is also printed out via serial when a new message is received. 

* please set the radio channel in the example sketch to your group number, otherwise radio traffic is going to cause problems!
* Each transciever module on a channel should have a unique ID, which should just be a number between 0 - 255 (one byte). You need to set this radio's ID so that the transmitter sends to the correct target ID.

```cpp
/*  nRF24L01 Receive Example
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

byte data_rx = 0; // this variable will be updated with the incoming message

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
```
### Button Transmit

This example requires a button attached to pin 2. A message is sent using the nRF24L01 when the button is pressed. The message can be used to activate something, such as the LED in the example below:

```cpp
/*  nRF24L01 Transmit on Button Press Example
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

byte mode = 0; // Mode can be the variable to be transmitted 

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
      delay(20); // A short delay to avoid 'bouncing'
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
```

### LED Receive

This sketch requires an LED attached to pin 2. On receipt of a message over radio, the LED will be activated or deactivated.

```cpp
/*  NRF24L01 Radio controlled LED example
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

// LED
byte ledpin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);

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
    if(data_rx == 1) {
      msg += "turn on the LED";
      digitalWrite(ledpin, HIGH);
    }
    else {
      msg += "turn off the LED";
      digitalWrite(ledpin, LOW);
    }

    Serial.println(msg); // print the message
  }

}
```

### Transceiver

A transceiver is a device that can both send and receive signals - the nRF24L01 is a radio transceiver module, meaning a sketch running on an Arduino or similar device can be used to both send and receive radio signals. In the example below, two Arduinos can be used to talk to each other. A button should be added to pin 2 on both Arduinos. You will need to make sure you change the send_id and receive_id on each Arduino - make sure each Arduino is sending data to the right ID!

When the button is pressed on one of the Arduinos, a radio packet (the number '255') will be sent to the other. On receipt of this message, the receiver will generate a random number (between 1 and 50) and send it back to the other, which will print the message over serial to confirm it is received. 

Try this program out, then as a challenge: 

* why don't you try adding an LED to each Arduino and get the LED to blink a number of times in response to the random number?
* How can you make this system more robust, so it will try again if a message does not send?
* You could load both Arduinos with identical code, but use a digital pin to define the radio ID in 'setup' - how could you do this?

```cpp
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
byte my_id = 0; // MAKE SURE YOU CHANGE THIS ON BOTH ARDUINOS - each Arduino should send to the other's ID
byte send_id = 1; // MAKE SURE YOU CHANGE THIS ON BOTH ARDUINOS - each Arduino should send to the other's ID
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
  if (!radio.init(radio_id, cepin, csnpin, NRFLite::BITRATE2MBPS, radio_chan)) {
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
```