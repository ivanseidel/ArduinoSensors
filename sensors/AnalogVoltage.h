/*
 	AnalogVoltage.h - A class to help with Analog Input pins.

 	This class extends AnalogIn, and is usefull for converting
 	ADC values to real Voltage values, making it easy to work with
 	many Arduino Libraries 

	https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, September, 2013.
	Released into the public domain.
*/

#ifndef AnalogVoltage_h
#define AnalogVoltage_h

#include <Arduino.h>

#include <AnalogIn.h>
#include <ArduinoSensors.h>
#include <interfaces/VoltageInterface.h>

// Voltage that indicates no voltage assignment
#define NO_VOLTAGE	-9999

class AnalogVoltage: public AnalogIn, public VoltageInterface
{
protected:
	// Cached value of the Voltage
	float voltage;

	/*
		Number of bits used in the conversion
		(We need to know this, in order to scale
		to the maximum value of the ADC)

		TODO: Currently only working with 10bits AD
	*/
	// int analogResolution;
	// long cachedMaxValue;
	static const long cachedMaxValue = 1023;

	/*
		This parameter is used for changing the AREF of the arduino.
		ARFE is based initialy, by the definition ADC_DEFAULT_AREF,
		however you can change it to whatever you want...
	*/
	static float defaultAnalogReference;

	/*
		Convertas an AD value to voltage, based on the maxVoltage 
		and minVoltage values
	*/
	virtual float convert(long ADValue){
		float percentage = (((float) ADValue) / cachedMaxValue);

		return (maxVoltage - minVoltage) * percentage + minVoltage;
	}

public:

	/*
		Initialize the Input pin, with the desired max and min voltages

		_inPin: Analog input pin to read value

		In order to initialize the object, a maximum voltage must be
		assigned, so that the AD conversion can be performed, and a
		voltage can be found.

		That voltage usualy is the Arduino Voltage (3.3v, 5v...), so we
		can use the definition ADC_DEFAULT_MAX_VOLTAGE to know that voltage.
		So: ADC = 0 			-> outputs 0.0;
			ADC = MAX_ADC_VALUE	-> outputs ADC_DEFAULT_AREF;
		
		However, you can also change the ADC value, or change the AREF pin,
		witch would cause a failure on the conversion.

		In case you have changed the AREF, make shure that you initialize
		this object with v1 being the maximum voltage, and v2 left unasigned.
		So: ADC = 0 			-> outputs 0.0;
			ADC = MAX_ADC_VALUE	-> outputs maxVoltage (v1);

		In other cases, you might be using some external hardware to convert
		a battery voltage into a lower voltage, witch can cause the function
		to shift up or down (in case of diodes, Amp. Op., ...). 

		If that's the case, and you need to add a "base value" to the conversion,
		use it as the FIRST parameter, and the maximum value as the SECOND one.
		So: ADC = 0				-> outputs minVoltage (v1);
			ADC = MAX_ADC_VALUE	-> outputs maxVoltage (v2);
	*/
	AnalogVoltage(int _inPin = -1, float v1 = -9999, float v2 = -9999): AnalogIn(_inPin){
		voltage = 0;

		if(v1 == NO_VOLTAGE && v2 == NO_VOLTAGE){
			// Both voltages are unasigned
			minVoltage = 0.0;
			maxVoltage = defaultAnalogReference;
		}else if(v2 == NO_VOLTAGE){
			minVoltage = 0.0;
			maxVoltage = v2;
		}else{
			minVoltage = v1;
			maxVoltage = v2;
		}
	}

	/*
		Only returns the cached Voltage
	*/
	virtual float getVoltage(){
		return voltage;
	}

	/*
		Reads voltage, converts and caches it
	*/
	virtual float readVoltage(){
		read();

		voltage = convert(getValue());

		return getVoltage();
	}

	/*
		Called when this Thread is runned
	*/
	virtual void run(){
		readVoltage();

		runned();
	}
};

// Defines the default analogReference value to the arduino voltage
float AnalogVoltage::defaultAnalogReference = ADC_DEFAULT_AREF * (1 - ADC_VOLTAGE_PROP);


#endif