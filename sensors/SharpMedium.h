#ifndef SharpMedium_h
#define SharpMedium_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpMedium: public AnalogIn, public DistanceInterface
{
protected:
	long distance;

public:
	SharpMedium(int _pin){
		setup(_pin);

		distance = 0;

		minVal = 6;
		maxVal = 45;
	}

	long getDistance(){
		return distance;
	}

	long readDistance(){
		return read();
	}

	virtual long read(){
		AnalogIn::read();

		// Calculate distance
		// distance = 9019.84/(value + 20.36);
		distance = 6570.66/(value - 16.0);

		// Filter lower and high values
		distance = max(minVal, min(maxVal,distance));

		return distance;
	}

};

#endif