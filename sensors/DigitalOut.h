#ifndef DigitalOut_h
#define DigitalOut_h

#include "Arduino.h"

class DigitalOut
{
protected:
	int outPin;

	bool invert;

public:

	// Pin: Analog pin to read value
	// Invert: Should invert the output signal?
	DigitalOut(int _outPin, bool _invert = false){
		pinMode(outPin, OUTPUT);

		outPin = _outPin;
		invert = _invert;

		turn(LOW);
	}

	bool operator==(bool other){
		return ((digitalRead(outPin) ^ invert) == other);
	}

	void operator=(bool on){
		turn(HIGH);
	}

	void turn(bool on){
		#if defined(__arm__)
			// A little bit faster than digitalWrite
			PIO_SetOutput( g_APinDescription[outPin].pPort, g_APinDescription[outPin].ulPin, on ^ invert, 0, PIO_PULLUP ) ;
		#else
			digitalWrite(outPin, on ^ invert);
		#endif
	}

	void turnOn(){
		turn(HIGH);
	}

	void turnOff(){
		turn(LOW);
	}

};

#endif