/*
 	VoltageInterface.h - An Interface for Voltage sensors and Analog pins

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef VoltageInterface_h
#define VoltageInterface_h

class VoltageInterface
{
public:
	// This is the minimum range of the sensor
	float minVoltage;
	// This is the maximum range of the sensor
	float maxVoltage;

	// Should return the CACHED value of the sensor
	virtual float getVoltage();

	// Should READ and CACHE the value of the sensor
	virtual float readVoltage();
};

#endif