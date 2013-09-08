/*
 	TemperatureInterface.h - An Interface for sensors that measure Temperature

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef TemperatureInterface_h
#define TemperatureInterface_h

class TemperatureInterface
{
public:
	// This is the minimum range of the sensor
	float minTemperature;
	// This is the maximum range of the sensor
	float maxTemperature;

	// Should return the CACHED value of the sensor
	virtual float getTemperature();

	// Should READ and CACHE the value of the sensor
	virtual float readTemperature();
};

#endif