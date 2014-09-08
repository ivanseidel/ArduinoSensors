/*
 	DigitalOut.h - A class to help with Digital Output pins.

 	Instantiate:
 		DigitalOut myOutputPin(13);

	Set output:
 		myOutputPin = HIGH;
 		myOutputPin.turn(HIGH);
 		myOutputPin.turnOn();

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef DigitalOut_h
#define DigitalOut_h

#include "Arduino.h"

class DigitalOut
{
protected:
	/*
		The number of the pin
	*/
	int outPin;

	/*
		This attribute is a flag to indicate if
		the output will be inverted.

		If set to true, then turning "ON" actualy
		turns OFF.

		(Eg. of usage: Relays that activate on LOW)
	*/
	bool invert;

public:

	/*
		Pin: Analog pin to read value
		Invert: Should invert the output signal?
	*/
	DigitalOut(int _outPin, bool _invert = false){
		pinMode(_outPin, OUTPUT);

		outPin = _outPin;
		invert = _invert;

		turn(LOW);
	}

	/*
		Return the output signal of this pin
	*/
	virtual operator bool(){
		return (digitalRead(outPin) ^ invert);
	}

	/*
		Set the output to the desired value;
		Use as:
			digitalOutObject = true; (Turns on)
			digitalOutObject = HIGH; (Turns on)
			digitalOutObject = LOW; (Turns off)
	*/
	virtual void operator=(bool on){
		turn(on);
	}

	/*
		Set the output to HIGH or LOW depending on the boolean on
		and the invert attribute
	*/
	virtual void turn(bool on){
		#if defined(__arm__)
			// A little bit faster than digitalWrite
			PIO_SetOutput( g_APinDescription[outPin].pPort, g_APinDescription[outPin].ulPin, on ^ invert, 0, PIO_PULLUP ) ;
		#else
			digitalWrite(outPin, on ^ invert);
		#endif
	}

	/*
		Facilitate methods
	*/
	virtual void turnOn(){
		turn(HIGH);
	}
	virtual void turnOff(){
		turn(LOW);
	}

};

#endif