/*
 	HMC6352.h - Honeywell HMC6352 Compass sensor library

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Datasheet: https://www.sparkfun.com/datasheets/Components/HMC6352.pdf

	Created by 	Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef EZUltrasonic_h
#define EZUltrasonic_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

#include <math.h>

class EZUltrasonicAnalog: public DistanceInterface
{
protected:

	// Cached value of the Distance
	float distance;

	/*
		To prevent Interruptions from reading the distance while it's
		being converted, we save it localy and then change the global
	*/
	float tmpDistance;
	
	AnalogIn analogPin;

	void processValue(){
		long adc = analogPin.read();

		// Calculate real Centimiter value
		tmpDistance = adc * 13.0 / 10;
		
		distance = tmpDistance;
	}

public:
	EZUltrasonicAnalog(int _pin){
		analogPin = AnalogIn(_pin);

		distance = 0;
	}

	virtual float getDistance(){
		return distance;
	}

	virtual float readDistance(){
		// Read ADC and converts
		processValue();

		return getDistance();
	}

};

#endif