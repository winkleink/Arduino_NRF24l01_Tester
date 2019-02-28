# Arduino_NRF24l01_Tester
Test code for NRF24l01 with Arduino Uno/Nano

From the code comments.

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
