/*
 	DistanceInterface.h - An Interface for sensors that measure Distance

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef DistanceInterface_h
#define DistanceInterface_h

class DistanceInterface
{
public:
	// This is the minimum range of the sensor
	float minDistance;
	// This is the maximum range of the sensor
	float maxDistance;

	// Should return the CACHED value of the sensor
	virtual float getDistance();

	// Should READ and CACHE the value of the sensor
	virtual float readDistance();
};

#endif