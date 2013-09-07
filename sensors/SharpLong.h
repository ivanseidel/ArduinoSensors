/*
 	SharpLong.h - Sharp Infrared Distance Sensor GP2Y0A02YK0F Library

	Working range: 30 - 90

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes,
				André Seidel Oliveira, June, 2013.
	Released into the public domain.
*/

#ifndef SharpLong_h
#define SharpLong_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpLong: public AnalogIn, public DistanceInterface
{
protected:
	// Cached Distance of the sensor
	long distance;

public:
	/*
		Initialize 
	*/
	SharpLong(int _pin): AnalogIn(_pin){
		distance = 0;

		minVal = 30;
		maxVal = 90;
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
		// distance = 16356.00 / (value - 57.35);
		distance = 16356.00 / (value - 57.35);

		if(distance > 60)
			distance = 17985.61 / (value - 30.18);

		// Filter lower and high values
		if(distance <= 0)
			distance = maxVal;
		
		distance = max(minVal, min(maxVal,distance));

		return distance;
	}

};

#endif