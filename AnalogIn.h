/*
 	AnalogIn.h - A class to help with Analog Input pins.

	https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef AnalogIn_h
#define AnalogIn_h

#include "Arduino.h"

#include <Thread.h>

class AnalogIn: public Thread
{
protected:
	// Cached value of the AnalogInput
	long value;

	// Threshold value
	long threshold;

	// Pin to read to
	int inPin;

public:

	/*
		Initialize the Input pin.

		_inPin: Analog input pin to read value
	*/
	AnalogIn(int _inPin = -1){
		setup(_inPin);

		Thread::Thread();
		setInterval(20);
	}

	virtual void setup(int _inPin){
		if(_inPin < 0)
			return;

		inPin = _inPin;

		pinMode(inPin, INPUT);

		value = 0;
		threshold = 0;
	}

	/*
		Use this operator to compare with boolean values.

		This will return if the threshold is or not equeal to
		the other boolean.

		Eg.: (inputPin == HIGH)
				is the same as
			 (inputPin.aboveThreshold() == true)
	*/
	virtual bool operator==(bool other){
		return aboveThreshold() == other;
	}

	/*
		Called when this Thread is runned
	*/
	virtual void run(){
		read();
		runned();
	}

	/*
		Reads the ADC and avarage it three times (reduce noise)
		Saves localy and then returns it.
	*/
	virtual long read(){
		value = (analogRead(inPin) + analogRead(inPin))/2;
		return getValue();
	}

	/*
		Only returns the value, it doesen't samples the ADC
	*/
	virtual long getValue(){
		return value;
	}

	/*
		Set a threshold for comparation
	*/
	virtual void setThreshold(long _threshold){
		threshold = _threshold;
	}

	/*
		Check if it's avove the selected threshold
	*/
	virtual bool aboveThreshold(){
		return read() > threshold;
	}

	/*
		Check if it's below the selected threshold
	*/
	virtual bool belowThreshold(){
		return read() < threshold;
	}

};

#endif