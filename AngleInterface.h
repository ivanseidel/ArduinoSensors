/*
 	AngleInterface.h - An Interface for sensors that measure Angle

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef AngleInterface_h
#define AngleInterface_h

class AngleInterface
{
public:
	// Should return the CACHED value of the sensor
	virtual float getAngle();
	
	// Should READ and CACHE the value of the sensor
	virtual float readAngle();
};

#endif