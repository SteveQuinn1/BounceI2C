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

#include <Wire.h>
#include <BounceI2C.h>

#define NXP_PCF8574P_ADDRESS 0x3F
const int buttonPin1 = 1;          // P1 of NXP_PCF8574P
const int buttonPin2 = 2;          // P2 of NXP_PCF8574P
const int ledPin1    = 6;          // P6 of NXP_PCF8574P
const int ledPin2    = 7;          // P7 of NXP_PCF8574P
#define NXP_PCF8574P_INPUT_MASK 0b00000110
bool isOn1 = false;
bool isOn2 = false;
BounceI2C myButtonP1 = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);
BounceI2C myButtonP2 = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);
BounceI2C port       = BounceI2C(NXP_PCF8574P_ADDRESS, NXP_PCF8574P_INPUT_MASK);
#define TOGGLE_BIT(p,whichBit) ((p) ^=  (1    << (whichBit)))

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Serial.println("BounceI2CTest Ready");

  myButtonP1.attach(buttonPin1);
  myButtonP1.interval(5);
  myButtonP2.attach(buttonPin2);
  myButtonP2.interval(5);
}




void loop()
{
  if(myButtonP1.update() && myButtonP1.read() == HIGH){ // Read button status and check for HIGH or LOW
    if(isOn1 == false){
      Serial.println("Pressed 1");
      isOn1 = true;
      port.digitalWrite(ledPin1, LOW);
    } else {
      Serial.println("Released 1");
      isOn1 = false;
      port.digitalWrite(ledPin1, HIGH);
    }
  }

  if(myButtonP2.update() && myButtonP2.read() == HIGH){ // Read button status and check for HIGH or LOW
    if(isOn2 == false){
      Serial.println("Pressed 2");
      isOn2 = true;
      port.digitalWrite(ledPin2, LOW);
    } else {
      Serial.println("Released 2");
      isOn2 = false;
      port.digitalWrite(ledPin2, HIGH);
    }
  }

}




/*
  {
    Wire.requestFrom((int)NXP_PCF8574P_ADDRESS, (int)1);
    uint8_t val = (uint8_t) Wire.read();
    Serial.print("Read : ");
    Serial.println(val,HEX);
  }
  delay(1000);           // wait 5 seconds for next scan

  {
    Wire.requestFrom((int)NXP_PCF8574P_ADDRESS, (int)1);
    uint8_t val = (uint8_t) Wire.read();
    TOGGLE_BIT(val,7);
    Serial.print("Write : ");
    Serial.println(val,HEX);
    Wire.beginTransmission((int)NXP_PCF8574P_ADDRESS); 
    Wire.write(val | NXP_PCF8574P_INPUT_MASK);
    Wire.endTransmission();
  }
  delay(1000);           // wait 5 seconds for next scan
*/

