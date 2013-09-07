/*
 	AnalogIn.h - A class to help with Analog Input pins.

	https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef AnalogIn_h
#define AnalogIn_h

#include "Arduino.h"

#include "Thread.h"

class AnalogIn: public Thread
{
protected:
	long 	value,	// Cached Last value
			threshold;

	int inPin;
public:

	// Pin: Analog pin to read value
	// lightPin: Pin connected to the LED
	AnalogIn(int _inPin = -1){
		setup(_inPin);

		Thread::Thread();
	}

	virtual void setup(int _inPin){
		if(_inPin < 0)
			return;

		inPin = _inPin;

		pinMode(inPin, INPUT);

		value = 0;
		threshold = 0;
	}

	virtual bool operator==(bool other){
		return aboveThreshold() == other;
	}

	// Called when this Thread is runned
	virtual void run(){
		read();
		runned();
	}

	virtual long read(){
		value = (analogRead(inPin) + analogRead(inPin) + analogRead(inPin))/3;
		return value;
	}

	virtual long getValue(){
		return value;
	}

	virtual void setThreshold(long _threshold){
		threshold = _threshold;
	}

	virtual bool aboveThreshold(){
		return read() > threshold;
	}

	virtual bool belowThreshold(){
		return read() < threshold;
	}

};

#endif