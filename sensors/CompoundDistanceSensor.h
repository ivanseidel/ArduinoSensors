/*
	
*/

#ifndef CompoundDistanceSensor_h
#define CompoundDistanceSensor_h

#include <Thread.h>
#include <DistanceInterface.h>

class CompoundDistanceSensor: public Thread, public DistanceInterface
{
protected:
	long 	value;	// Cached Last value;

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

	virtual long readDistance(){
		if(!dist1 || !dist2)
			return -1;

		long val1, val2;
		val1 = dist1->readDistance();
		val2 = dist2->readDistance();

		value = calculate(val1, val2);

		return value;
	}

	virtual long calculate(long val1, long val2){
		if(onCalculate)
			value = onCalculate(val1, val2);

		return value;
	}

	virtual long getDistance(){
		return value;
	}

};

#endif