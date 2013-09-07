#ifndef SharpLongMod_h
#define SharpLongMod_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

class SharpLongMod : public AnalogIn, public DistanceInterface
{
protected:
	long distance;

public:
	SharpLongMod(int _pin){
		setup(_pin);

		distance = 0;
	}

	virtual long getDistance(){
		return distance;
	}

	virtual long readDistance(){
		return read();
	}

	virtual long read(){
		AnalogIn::read();
		
		distance = 16356.00 / (value - 57.35);
		
		return distance;
	}

};

#endif