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

#include <sensors/SharpInterface.h>

class SharpShort: public SharpInterface
{
protected:
	/*
		Convert the value
	*/
	virtual float convertDistance(float value){
		// distance = 11024.98 / (value +100.24) - 4.0;
		float converted = 5489.72 / (value - 40.03);

		// Filter lower and high values
		converted = max(minDistance, min(maxDistance,converted));

		return converted;
	}

public:
	/*
		Initialize 
	*/
	SharpShort(int _pin): SharpInterface(_pin){
		minDistance = 7;
		maxDistance = 30;
	}

};

#endif