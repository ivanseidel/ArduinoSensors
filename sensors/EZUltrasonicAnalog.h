/*
 	HMC6352.h - Honeywell HMC6352 Compass sensor library

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Datasheet: https://www.sparkfun.com/datasheets/Components/HMC6352.pdf

	Created by 	Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef EZUltrasonic_h
#define EZUltrasonic_h

#include <Thread.h>
#include <Arduino.h>

#include <ArduinoSensors.h>
#include <sensors/AnalogVoltage.h>
#include <AnalogIn.h>
#include <interfaces/DistanceInterface.h>

#include <math.h>

/*
	Those definitios are used to know the correct
	proportional to the VCC value, per cm
*/
#define VCC					1.0

#define LV_EZ0				toCentimeters(VCC/512)
#define LV_EZ1				toCentimeters(VCC/512)
#define LV_EZ2				toCentimeters(VCC/512)
#define LV_EZ3				toCentimeters(VCC/512)
#define LV_EZ4				toCentimeters(VCC/512)

#define XL_EZ0				VCC/1024
#define XL_EZ1				VCC/1024
#define XL_EZ2				VCC/1024
#define XL_EZ3				VCC/1024
#define XL_EZ4				VCC/1024
#define XL_EZL0				VCC/1024

class EZUltrasonicAnalog: public Thread, public DistanceInterface
{
protected:

	// Cached value of the Distance
	float distance;

	/*
		To prevent Interruptions from reading the distance while it's
		being converted, we save it localy and then change the global
	*/
	float tmpDistance;
	
	// Object that reads from Analog pin
	AnalogIn analogPin;

	// Factor to scale distance
	float scaleFactor;

	float convert(float voltage){
		Serial.println(voltage);
		// Calculate real Centimiter value
		return tmpDistance = voltage * 10;//(voltage / 512) * (scaleFactor * analogPin.maxVoltage);
	}

public:
	EZUltrasonicAnalog(int _pin, float _scaleFactor = LV_EZ0){
		analogPin = AnalogIn(_pin);

		distance = 0;

		// TODO: Modify range acordingly to the device selected
		minDistance = 10;
		maxDistance = 500;

		scaleFactor = _scaleFactor;

		Thread::Thread();
		setInterval(20);
	}

	/*
		Returns the cached distance
	*/ 
	virtual float getDistance(){
		return distance;
	}

	/*
		Sample ADC, converts to CM and returns cached distance
	*/
	virtual float readDistance(){
		// Read ADC and converts
		// distance = convert(analogPin.readVoltage());
		distance = analogPin.read();

		return getDistance();
	}

	/*
		Thread callback
	*/
	virtual void run(){
		readDistance();

		runned();
	}

};

#endif