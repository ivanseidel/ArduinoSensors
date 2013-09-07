/*
 	SharpMedium.h - Sharp Infrared Distance Sensor GP2Y0A21YK Library

	Working range: 6 - 45

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes,
				André Seidel Oliveira, June, 2013.
	Released into the public domain.
*/

#ifndef SharpMedium_h
#define SharpMedium_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpMedium: public AnalogIn, public DistanceInterface
{
protected:
	// Cached Distance of the sensor
	long distance;

public:
	/*
		Initialize 
	*/
	SharpMedium(int _pin){
		// Calls constructor of AnalogIn
		AnalogIn::AnalogIn(_pin);

		distance = 0;

		minVal = 6;
		maxVal = 45;
	}

	/*
		Returns the CACHED distance
	*/
	long getDistance(){
		return distance;
	}

	/*
		Reads, CACHE and returns the cached value
	*/
	long readDistance(){
		return read();
	}

	/*
		Reads, CACHE and returns the cached value
	*/
	virtual long read(){
		AnalogIn::read();

		// Calculate distance
		// distance = 9019.84/(value + 20.36);
		distance = 6570.66/(value - 16.0);

		// Filter lower and high values
		distance = max(minVal, min(maxVal,distance));

		return distance;
	}

};

#endif