/*
 	SharpInterface.h - Sharp Infrared Distance Sensor Interface

	This class provides basic mathods for all Sharp Sensors.

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes,
				André Seidel Oliveira, June, 2013.
	Released into the public domain.
*/

#ifndef SharpInterface_h
#define SharpInterface_h

#include <AnalogIn.h>
#include <Thread.h>
#include <interfaces/DistanceInterface.h>

class SharpInterface: public Thread, public DistanceInterface
{
protected:
	// Cached Distance of the sensor
	float distance;

	// AnalogInput of the sensor
	AnalogIn analogPin;

	/*
		This method is responsable for converting the
		corresponding ADC value into CM value and returning
	*/
	virtual float convertDistance(float value){return value;}

public:
	/*
		Initialize 
	*/
	SharpInterface(int _pin){
		analogPin = AnalogIn(_pin);

		distance = 0;

		Thread::Thread();
		setInterval(20);
	}

	/*
		Returns the CACHED distance
	*/
	virtual float getDistance(){
		return distance;
	}

	/*
		Reads, CACHE and returns the cached value
	*/
	virtual float readDistance(){
		distance = convertDistance(analogPin.read());

		return getDistance();
	}

	/*
		Thread callback
	*/
	virtual void run(){
		readDistance();
		
		runned();
	}

};

#endif