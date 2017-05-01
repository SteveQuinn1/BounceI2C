// Please read BounceI2C.h for information about the license and authors

/*
 Modified to work with NXP PCF8574P I2C by SteveQuinn 2017
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "BounceI2C.h"
#include <Wire.h>

#define DEBOUNCED_STATE 0
#define UNSTABLE_STATE  1
#define STATE_CHANGED   3

//#define DEBUG_BOUNCE_I2C

#define B_SET_BIT(p,whichBit)    ((p) |=  (1    << (whichBit)))
#define B_CLEAR_BIT(p,whichBit)  ((p) &= ~((1)  << (whichBit)))

BounceI2C::BounceI2C(uint8_t address, uint8_t input_mask)
    : previous_millis(0)
    , interval_millis(10)
    , state(0)
    , pin(0)
{
	_address = address;
	_input_mask = input_mask;
}

void BounceI2C::attach(int pin) {
    this->pin = pin;
    state = 0;
    Wire.requestFrom((int)_address, (int)1);
    uint8_t val = (uint8_t) Wire.read();
    Wire.beginTransmission((int)_address);
    Wire.write(val | _input_mask);
    Wire.endTransmission();
    if (val & _BV(pin)) {
        state = _BV(DEBOUNCED_STATE) | _BV(UNSTABLE_STATE);
    }
#ifdef BOUNCE_LOCK_OUT
    previous_millis = 0;
#else
    previous_millis = millis();
#endif
}

// Just for compatability
void BounceI2C::attach(int pin, int mode){
  //pinMode(pin, mode);

  this->attach(pin);
}

void BounceI2C::interval(uint16_t interval_millis)
{
    this->interval_millis = interval_millis;
}

bool BounceI2C::update()
{
#ifdef BOUNCE_LOCK_OUT
    state &= ~_BV(STATE_CHANGED);
    // Ignore everything if we are locked out
    if (millis() - previous_millis >= interval_millis) {
		Wire.requestFrom((int)_address, (int)1);
		uint8_t val = (uint8_t) Wire.read();
		Wire.beginTransmission((int)_address);
		Wire.write(val | _input_mask);
		Wire.endTransmission();
    	bool currentState = val & _BV(pin);
        //bool currentState = digitalRead(pin);
        if ((bool)(state & _BV(DEBOUNCED_STATE)) != currentState) {
            previous_millis = millis();
            state ^= _BV(DEBOUNCED_STATE);
            state |= _BV(STATE_CHANGED);
        }
    }
    return state & _BV(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.
	Wire.requestFrom((int)_address, (int)1);
	uint8_t val = (uint8_t) Wire.read();
    Wire.beginTransmission((int)_address);
    Wire.write(val | _input_mask);
    Wire.endTransmission();
    bool readState = val & _BV(pin);
    //bool readState = digitalRead(pin);

    // Clear Changed State Flag - will be reset if we confirm a button state change.
    state &= ~_BV(STATE_CHANGED);

    if ( readState != (bool)(state & _BV(DEBOUNCED_STATE))) {
      // We have seen a change from the current button state.

      if ( millis() - previous_millis >= interval_millis ) {
	// We have passed the time threshold, so a new change of state is allowed.
	// set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
	// This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
	// Otherwise debounced state will not change again until bouncing is stable for the timeout period.
	state ^= _BV(DEBOUNCED_STATE);
	state |= _BV(STATE_CHANGED);
      }
    }

    // If the readState is different from previous readState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
    if ( readState != (bool)(state & _BV(UNSTABLE_STATE)) ) {
	// Update Unstable Bit to macth readState
        state ^= _BV(UNSTABLE_STATE);
        previous_millis = millis();
    }
    // return just the sate changed bit
    return state & _BV(STATE_CHANGED);
#else
    // Read the state of the switch in a temporary variable.
	Wire.requestFrom((int)_address, (int)1);
	uint8_t val = (uint8_t) Wire.read();
	Wire.beginTransmission((int)_address);
	Wire.write(val | _input_mask);
	Wire.endTransmission();
	bool currentState = val & _BV(pin);
    //bool currentState = digitalRead(pin);
    state &= ~_BV(STATE_CHANGED);

    // If the reading is different from last reading, reset the debounce counter
    if ( currentState != (bool)(state & _BV(UNSTABLE_STATE)) ) {
        previous_millis = millis();
        state ^= _BV(UNSTABLE_STATE);
    } else
        if ( millis() - previous_millis >= interval_millis ) {
            // We have passed the threshold time, so the input is now stable
            // If it is different from last state, set the STATE_CHANGED flag
            if ((bool)(state & _BV(DEBOUNCED_STATE)) != currentState) {
                previous_millis = millis();
                state ^= _BV(DEBOUNCED_STATE);
                state |= _BV(STATE_CHANGED);
            }
        }

    return state & _BV(STATE_CHANGED);
#endif
}

bool BounceI2C::read()
{
	#ifdef DEBUG_BOUNCE_I2C
	Serial.print("read : ");
	Serial.println (state & _BV(DEBOUNCED_STATE));
	#endif
    return state & _BV(DEBOUNCED_STATE);
}

bool BounceI2C::rose()
{
    return ( state & _BV(DEBOUNCED_STATE) ) && ( state & _BV(STATE_CHANGED));
}

bool BounceI2C::fell()
{
    return !( state & _BV(DEBOUNCED_STATE) ) && ( state & _BV(STATE_CHANGED));
}


void BounceI2C::digitalWrite(int pin, int value){
	Wire.requestFrom((int)_address, (int)1);
	uint8_t val = (uint8_t) Wire.read();
	if (value == HIGH)
		B_SET_BIT(val,pin);
	else
		B_CLEAR_BIT(val,pin);
	Wire.beginTransmission((int)_address);
	Wire.write(val | _input_mask);
	Wire.endTransmission();
	#ifdef DEBUG_BOUNCE_I2C
	Serial.print("digitalWrite : ");
	Serial.println (val, HEX);
	#endif
}
