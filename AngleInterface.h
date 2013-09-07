/*
	
	This is a simple interface to hold common methods for angular sensors
	(such as compass, potenciometers...)

*/

#ifndef AngleInterface_h
#define AngleInterface_h

class AngleInterface
{
public:
	virtual float getAngle();
	virtual float readAngle();
};

#endif