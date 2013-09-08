/*
 	CompoundDistanceSensor.h - Integrate two Distance Sensor in a single sensor

	This Class helps to integrate two sensors that measure distance with diferent
	ranges in a single one, that provides a unique distance.

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/
#ifndef CompoundDistanceSensor_h
#define CompoundDistanceSensor_h

#include <Thread.h>
#include <interfaces/DistanceInterface.h>

class CompoundDistanceSensor: public Thread, public DistanceInterface
{
protected:
	// Cached Last value;
	long value;

	DistanceInterface *dist1, *dist2;

public:
	long (*onCalculate)(long, long);


	// Pin: Analog pin to read value
	// lightPin: Pin connected to the LED
	CompoundDistanceSensor(DistanceInterface *_dist1, DistanceInterface *_dist2){
		onCalculate = 0;
		dist1 = _dist1;
		dist2 = _dist2;

		minVal = min(dist1->maxVal, dist2->minVal);
		maxVal = max(dist1->maxVal, dist2->maxVal);
	}

	// Called when this Thread is runned
	virtual void run(){
		readDistance();
		runned();
	}

	virtual float readDistance(){
		if(!dist1 || !dist2)
			return -1;

		float val1, val2;
		val1 = dist1->readDistance();
		val2 = dist2->readDistance();

		value = calculate(val1, val2);

		return value;
	}

	virtual float calculate(float val1, float val2){
		if(onCalculate)
			value = onCalculate(val1, val2);

		return value;
	}

	virtual float getDistance(){
		return value;
	}

};

#endif