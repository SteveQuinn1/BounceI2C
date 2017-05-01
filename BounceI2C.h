/*
  The MIT License (MIT)

  Copyright (c) 2013 thomasfredericks
  Copyright (c) 2017 SteveQuinn

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
  Main code by Thomas O Fredericks (tof@t-o-f.info)
  Previous contributions by Eric Lowry, Jim Schimpf and Tom Harkaway
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 Modified to work with NXP PCF8574P I2C by SteveQuinn 2017
 Deltas maintainted by Steve Quinn <demomonkey@gmail.com>
*/

#ifndef BounceI2C_h
#define BounceI2C_h

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT

// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

#include <inttypes.h>

class BounceI2C
{
 public:
    // Create an instance of the bounce library and pass the I2C address of the NXP PCF8574P
    BounceI2C(uint8_t address, uint8_t input_mask);

    // Attach to a pin (and also sets initial state)
    void attach(int pin);

    // Attach to a pin (and also sets initial state) and sets pin to mode (INPUT/INPUT_PULLUP/OUTPUT)
    void attach(int pin, int mode);

    // Sets the debounce interval
    void interval(uint16_t interval_millis);

    // Updates the pin
    // Returns 1 if the state changed
    // Returns 0 if the state did not change
    bool update();

    // Returns the updated pin state
    bool read();

    // Returns the falling pin state
    bool fell();

	// Allows user to write to non input pins
	void digitalWrite(int pin, int value);

    // Returns the rising pin state
    bool rose();

    // Partial compatibility for programs written with Bounce version 1
    bool risingEdge() { return rose(); }
    bool fallingEdge() { return fell(); }

 protected:
    unsigned long previous_millis;
    uint16_t interval_millis;
    uint8_t state;
    uint8_t pin;
    uint8_t _address;
    uint8_t _input_mask;
};

#endif
