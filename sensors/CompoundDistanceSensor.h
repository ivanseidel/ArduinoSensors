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

		// Always set dist1 to have the lowest minimum range
		if(_dist1->minDistance < _dist2->minDistance){
			dist1 = _dist1;
			dist2 = _dist2;
		}else{
			dist1 = _dist2;
			dist2 = _dist1;
		}


		minVal = min(dist1->maxVal, dist2->minVal);
		maxVal = max(dist1->maxVal, dist2->maxVal);
	}

	/*
		This method will return the cached value of the calculated distance
	*/
	virtual float getDistance(){
		return value;
	}

	/*
		This method will read both distances, and send it to
		the callback function 'calculate'. The result will be
		saved and returned
	*/
	virtual float readDistance(){
		if(!dist1 || !dist2)
			return -1;

		float val1, val2;
		val1 = dist1->readDistance();
		val2 = dist2->readDistance();

		value = calculate(val1, val2);

		return getDistance();
	}

	virtual float calculate(float val1, float val2){
		if(onCalculate){
			value = onCalculate(val1, val2);
		}else{
			// default implementation, if callback not set
			if(val1 >= dist1->maxDistance)
				value = val2;
			else
				value = val1;
		}

		return value;
	}

	/*
		Thread implementation
	*/
	virtual void run(){
		readDistance();

		runned();
	}

};

#endif