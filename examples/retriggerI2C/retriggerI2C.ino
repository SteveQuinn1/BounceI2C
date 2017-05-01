// Copyright 2017 Steve Quinn
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
/* 
DESCRIPTION
====================
Example of the bounce library that shows how to retrigger an event when a button is held down.
In this case, the debug LED will blink every 500 ms as long as the button is held down.
Open the Serial Monitor (115200 baud) for debug messages.

*/

#include <Wire.h>
#include <BounceI2C.h>

#define NXP_PCF8574P_ADDRESS 0x3F
const int BUTTON_PIN = 1;          // P1 of NXP_PCF8574P
const int LED_PIN    = 6;          // P6 of NXP_PCF8574P
#define NXP_PCF8574P_INPUT_MASK 0b00000110
BounceI2C debouncer = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);
BounceI2C port      = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);



int buttonState;
unsigned long buttonPressTimeStamp;

int ledState = LOW;

void setup() {
  Wire.begin();  
  Serial.begin(115200);
  
  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);
  
  //Setup the LED
  port.digitalWrite(LED_PIN,ledState);
}



void loop() {
 // Update the debouncer and get the changed state
  boolean changed = debouncer.update();

  if ( changed ) {
       // Get the update value
     int value = debouncer.read();
    if ( value == HIGH) {
       ledState = LOW;
       port.digitalWrite(LED_PIN, ledState );
   
       buttonState = 0;
       Serial.println("Button released (state 0)");
   } else {
      ledState = HIGH;
      port.digitalWrite(LED_PIN, ledState );
     
      buttonState = 1;
      Serial.println("Button pressed (state 1)");
      buttonPressTimeStamp = millis();
   }
  }
  
  if  ( buttonState == 1 ) {
    if ( millis() - buttonPressTimeStamp >= 500 ) {
         buttonPressTimeStamp = millis();
         if ( ledState == HIGH ) ledState = LOW;
         else if ( ledState == LOW ) ledState = HIGH;
         port.digitalWrite(LED_PIN, ledState );
        Serial.println("Retriggering button");
    }
  }
}


