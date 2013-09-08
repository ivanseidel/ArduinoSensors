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

#include <sensors/SharpInterface.h>

class SharpLong: public SharpInterface
{
protected:
	/*
		Convert the value
	*/
	virtual float convertDistance(float value){
		// distance = 16356.00 / (value - 57.35);
		float converted = 16356.00 / (value - 57.35);

		// Filter lower and high values
		if(converted > 60)
			converted = 17985.61 / (value - 30.18);

		if(converted <= 0)
			converted = maxDistance;
		
		converted = max(minDistance, min(maxDistance,converted));

		return converted;
	}

public:
	/*
		Initialize 
	*/
	SharpLong(int _pin): SharpInterface(_pin){
		minDistance = 30;
		maxDistance = 90;
	}

};

#endif