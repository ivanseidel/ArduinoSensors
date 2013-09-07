#ifndef SharpShort_h
#define SharpShort_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpShort: public AnalogIn, public DistanceInterface
{
protected:
	long distance;

public:
	SharpShort(int _pin){
		setup(_pin);

		distance = 0;

		minVal = 7;
		maxVal = 30;
	}

	virtual long getDistance(){
		return distance;
	}

	virtual long readDistance(){
		return read();
	}

	virtual long read(){
		AnalogIn::read();

		// Calculate distance
		// distance = 11024.98 / (value +100.24) - 4.0;
		distance = 5489.72 / (value - 40.03);

		// Filter lower and high values
		distance = max(minVal, min(maxVal,distance));

		return distance;
	}

};

#endif