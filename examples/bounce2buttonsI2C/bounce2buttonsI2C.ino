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
 Simple example of the Bounce library that switches the debug LED when 
 either of 2 buttons are pressed.
 */
 
#include <Wire.h>
#include <BounceI2C.h>

#define NXP_PCF8574P_ADDRESS 0x3F
const int BUTTON_PIN_1 = 1;          // P1 of NXP_PCF8574P
const int BUTTON_PIN_2 = 2;          // P2 of NXP_PCF8574P
const int LED_PIN      = 6;          // P6 of NXP_PCF8574P
#define NXP_PCF8574P_INPUT_MASK 0b00000110
BounceI2C debouncer1 = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);
BounceI2C debouncer2 = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);
BounceI2C port       = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);


void setup() {
  Wire.begin();
  
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(BUTTON_PIN_1);
  debouncer1.interval(5); // interval in ms
  
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(5); // interval in ms
}


void loop() {
  // Update the Bounce instances :
  debouncer1.update();
  debouncer2.update();

  // Get the updated value :
  int value1 = debouncer1.read();
  int value2 = debouncer2.read();

  // Turn on the LED if either button is pressed :
  if ( value1 == LOW || value2 == LOW ) {
   port.digitalWrite(LED_PIN, LOW );
  } 
  else {
    port.digitalWrite(LED_PIN, HIGH );
  }

}


