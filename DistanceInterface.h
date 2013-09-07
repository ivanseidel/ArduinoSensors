/*
	
	This is a simple interface to hold common methods for distance sensors

*/

#ifndef DistanceInterface_h
#define DistanceInterface_h

class DistanceInterface
{
public:
	double minVal, maxVal;
	virtual long getDistance();
	virtual long readDistance();
};

#endif