/*
 	DigitalIn.h - A class to help with Digital Input pins.

 	Instantiate:
 		DigitalIn myInputPin(13);

	Check state:
 		if(myOutputPin == HIGH);
 		if(myOutputPin.getValue() == HIGH);

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef DigitalIn_h
#define DigitalIn_h

#include "Arduino.h"

class DigitalIn
{
protected:
	/*
		The number of the pin
	*/
	int pin;

	/*
		This attribute is a flag to indicate if
		the output will be inverted.

		If set to true, then turning "ON" actualy
		turns OFF.

		(Eg. of usage: Relays that activate on LOW)
	*/
	bool invert;

	// Cached value for the current pin
	bool digitalValue;

public:

	/*
		Pin: Analog pin to read value
		Invert: Should invert the output signal?
	*/
	DigitalIn(int _outPin, bool _invert = false){
		pin = _outPin;
		invert = _invert;

		pinMode(pin, INPUT);
	}

	/*
		Compares if the output signal of this pin,
		is equal to other boolean value
	*/
	virtual operator bool(){
		return readValue();
	}

	/*
		Returns the current state of the digital pin
	*/
	virtual bool readValue(){
		digitalValue = (digitalRead(pin) ^ invert);
		return getValue();
	}

	/*
		Returns the cached state of the digital pin
	*/
	virtual bool getValue(){
		return digitalValue;
	}
};

#endif