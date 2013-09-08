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

#include <sensors/SharpInterface.h>

class SharpMedium: public SharpInterface
{
protected:
	/*
		Convert the value
	*/
	virtual float convertDistance(float value){
		// distance = 9019.84/(value + 20.36);
		float converted = 6570.66/(value - 16.0);

		// Filter lower and high values
		converted = max(minDistance, min(maxDistance,converted));

		return converted;
	}

public:
	/*
		Initialize 
	*/
	SharpMedium(int _pin): SharpInterface(_pin){
		minDistance = 6;
		maxDistance = 45;
	}

};

#endif