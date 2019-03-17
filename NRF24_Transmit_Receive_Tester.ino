/*
Created by @winkleink 

Based on code from by Yvan / https://Brainy-Bits.com

This code is in licenced as MIT, so you can do as you wish with it.
The licence is only there so it's clear you can do what you want with it.
.
You can: copy it, use it, modify it, share it or just plain ignore it!
*/

// NRF24L01 Module Tester
/*
Using the NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
It's in the Arduino Library manager so you can install it from there.
If all you need is the library no need to go near GitHub

For this implementation on an Arduino Uno/Nano the pins used are
NRF           Arduino
VCC     3.3V
GND     GND
CSN     Digital 10
CE      Digital 9
MOSI    Digital 11
MISO    Digital 12
SCK     Digital 13 

LED on Pin 2
Switch on Pin 3 - Switch has internal pullup activated so when the switch is used it needs to connect Pin 3 to GND

Both Arduino/NRF24 start in transmit mode with their LEDs on.
On press the switch one it changes to receive mode.  The LED will flash 5 times quickly

Now you should have one with an LED on (transmit) and one with the LED off (receive)
On pressing the button on the transmit Arduino/NRF24 the transmit one will change to receive with the LED flashing 5 times and then goes off
At the same time the receive Arduino/NRF24 should receive the message transmitted and change to transmit mode with the LED on

Now the previous reciever is the transmitter (LED on) and the previous transmitter is the receiver (LED off after flashing 5 times)

*/

//Include needed Libraries at beginning
#include "nRF24L01.h" 
#include "RF24.h"
#include "SPI.h"

#define SwitchPin 3 // Arcade switch is connected to Pin 3 on NANO
#define LED_PIN 2 // Digital In (DI) of LED
int JumperPins[] = {7, 6, 5, 4};
int JumperValue = 0;

int tr = 1; // Using 1 for transmit and 0 for receive


// Variables related to the NRF24
RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 


int SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01
int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01



void setup(void){
Serial.begin(9600);
  pinMode(SwitchPin, INPUT_PULLUP); // Define the arcade switch NANO pin as an Input using Internal Pullups
  digitalWrite(SwitchPin,HIGH); // Set Pin to HIGH at beginning

  radio.begin(); // Start the NRF24L01
  radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
  radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive

  for(int i=0; i<4; i++)
    {
    pinMode(JumperPins[i],INPUT_PULLUP);
    }

  for(int i=0; i<4; i++)
    {
      if (digitalRead(JumperPins[i]) == HIGH)
        {
        JumperValue = JumperValue + (int(pow(2, (digitalRead(JumperPins[i])*i))+0.5));  // Needs the 0.5 added to ensure int rounds up
        Serial.println(pow(2, (digitalRead(JumperPins[i])*i)));
        Serial.println(JumperValue);
        }
    }
  JumperValue = 15 - JumperValue;
  Serial.println(JumperValue);

  if (JumperValue==0)
    {
    tr = 1; // Transmitter
    }
   else
    {
    tr = 0; // Receiver
    }

Serial.println(tr);
}


void loop(void){


// Transmitter. Waiting for button to be pressed and then send the message and turn off the LED and change to Receiver

// When in transmit mode stop listening and set LED on.
if (tr==1){
  Serial.println("Changing to Transmit mode and waiting for button to be pressed");
  radio.stopListening(); // Stop listening as going to send message when button pressed
  digitalWrite(LED_PIN, HIGH);
}

// When the button is pressed send a message and change to receive mode
while (tr == 1){
  if (digitalRead(SwitchPin) == LOW){ // If Switch is Activated
  SentMessage[0] = 111;
  radio.write(SentMessage, 1); // Send value through NRF24L01
  tr = 0;  // Change to receive mode
  }
  delay(20);

}


// Receiver. Wait for message to be received and turn on LED and change to Transmitter

// When changing to receiver flash LED 5 times and then have it off
if (tr==0){
  Serial.println("Changing to Receive mode waiting for a message to be received");
  SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01
  ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01
  radio.startListening(); // Be a receiver and listen for the message from the transmitter
  digitalWrite(LED_PIN, LOW); // Turn off the LED as no message recieved
  
    for(int x = 0; x<5; x++){
    digitalWrite(LED_PIN,HIGH);
    delay(200);
    digitalWrite(LED_PIN,LOW);
    delay(50);
  }
}


// wait for message and when received change to transmit mode
while (tr == 0){
   while (radio.available() && tr==0)
    {
      radio.read(ReceivedMessage, 1); // Read information from the NRF24L01
  
      if (ReceivedMessage[0] == 111) // Indicates switch is pressed
        {
        tr = 1;
        delay(20);
        }
    }
}

// End of VOID Function
}
