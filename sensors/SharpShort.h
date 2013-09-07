/*
 	SharpShort.h - Sharp Infrared Distance Sensor GP2D120XJ00F Library

	Working range: 7 - 30

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes,
				André Seidel Oliveira, June, 2013.
	Released into the public domain.
*/

#ifndef SharpShort_h
#define SharpShort_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpShort: public AnalogIn, public DistanceInterface
{
protected:
	// Cached Distance of the sensor
	long distance;

public:
	/*
		Initialize 
	*/
	SharpShort(int _pin){
		// Calls constructor of AnalogIn
		AnalogIn::AnalogIn(_pin);

		distance = 0;

		minVal = 7;
		maxVal = 30;
	}

	/*
		Returns the CACHED distance
	*/
	virtual long getDistance(){
		return distance;
	}

	/*
		Reads, CACHE and returns the cached value
	*/
	virtual long readDistance(){
		return read();
	}

	/*
		Reads, CACHE and returns the cached value
	*/
	virtual long read(){
		AnalogIn::read();

		// Calculate distance
		// distance = 11024.98 / (value +100.24) - 4.0;
		distance = 5489.72 / (value - 40.03);

		// Filter lower and high values
		distance = max(minVal, min(maxVal,distance));

		return distance;
	}

};

#endif