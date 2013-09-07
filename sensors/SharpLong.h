#ifndef SharpLong_h
#define SharpLong_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpLong: public AnalogIn, public DistanceInterface
{
protected:
	long distance;

public:
	SharpLong(int _pin){
		setup(_pin);

		distance = 0;

		minVal = 30;
		maxVal = 90;
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
		// distance = 16356.00 / (value - 57.35);
		distance = 16356.00 / (value - 57.35);

		if(distance > 60)
			distance = 17985.61 / (value - 30.18);

		// Filter lower and high values
		if(distance <= 0)
			distance = maxVal;
		
		distance = max(minVal, min(maxVal,distance));

		return distance;
	}

};

#endif