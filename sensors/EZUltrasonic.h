#ifndef EZUltrasonic_h
#define EZUltrasonic_h

#include <AnalogIn.h>
#include <DistanceInterface.h>

#include <math.h>

#define N_READS 6

class EZUltrasonic : public AnalogIn, public DistanceInterface
{
protected:
	long distance;
	long lastDist[N_READS];
	int curr;
	
public:
	EZUltrasonic(int _pin){
		setup(_pin);

		distance = 0;
		curr = 0;
	}

	long getDistance(){
		return distance;
	}

	long readDistance(){
		return read();
	}

	virtual long read(){
		AnalogIn::read();

		// Calculate real Centimiter value
		distance = (long) value * 13L / 10L;
		
		lastDist[curr++] = distance;
		curr %= N_READS;
		
		distance = 0;
		for (int i = 0; i < N_READS; i++)
			distance += lastDist[i];

		return distance / N_READS;
	}

};

#endif