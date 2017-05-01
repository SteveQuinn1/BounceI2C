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
Reports through serial (115200 baud) the time since 
a button press (transition from HIGH to LOW).

*/


#include <Wire.h>
#include <BounceI2C.h>

#define NXP_PCF8574P_ADDRESS 0x3F
const int BUTTON_PIN = 1;          // P1 of NXP_PCF8574P

#define NXP_PCF8574P_INPUT_MASK 0b00000110
BounceI2C debouncer = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);




unsigned long buttonPressTimeStamp;

void setup() {
  
  Serial.begin(115200);
  

  // After setting up the button, setup the Bounce instance :
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);
  
  
}

void loop() {
  
   // Update the Bounce instance :
  debouncer.update();

  // Call code if Bounce fell (transition from HIGH to LOW) :
  if ( debouncer.fell()  ) {;
  
    Serial.println( millis()-buttonPressTimeStamp );
     buttonPressTimeStamp = millis();
  
  }
  

}

