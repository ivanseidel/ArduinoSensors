/*
 	ArduinoSensors.h - Main ArduinoSensor file.

 	Provides repetitive code in pretty functions

 	Include this file in order to allow the compiler to discover the sub
 	folders and files of ArduinoSensors Library

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/


#ifndef ArduinoSensors_h
#define ArduinoSensors_h

#include <Arduino.h>

/*
	Simple conversion from float to String.

	float f: the float to be converted
	int decimals: number of decimal places
*/
String floatToString(float f, int decimals = 2){
	/*
		Transforms the floating point string process:
		eg.: using decimals = 2, input: 0.01
			1.23456 * (pow(10, 2+1)
				1.23456 * 1000 = 1234 (as a long)
			now, let's move digits asside:
			3 goes to 4, 2 goes to 3.
			Stages:		"1234"
						"1233"
						"1223"
			Not, put a dot in the place of the separator
			"1.23"

	*/
	long fLong = f*(pow(10,decimals+1));
	String longString = String(fLong);
	int len = longString.length();

	for(int i = 0; i < decimals; i++){
		longString[len - 1 - i] = longString[len - 2 - i];
	}
	longString[len - decimals - 1] = '.';

	return longString;
}

/*
	Convert an angle in Degrees to Radians
*/
float toRads(double angle){
	return angle * M_PI/180.0;
}

/*
	Convert an angle in Radians to Degrees
*/
float toDegs(double angle){
	return angle * 180.0/M_PI;
}

/*
	This function is usefull for fixing realtive angles.
	If you input anything from -180 to 180, it will return the same.

	However, if you input for example, 200, it will return -160
	Input |	Output
	0 		0
	180		180
	-180	-180
	181		-179
	-181	179
	360		0
	540		-180

*/
float fixDegrees(float angle){
	while (angle < -180.0)
		angle += 360.0;

	while (angle > 180.0)
		angle -= 360.0;

	return angle;
};


/*
	This is the same as the above, but working with Radians.
	(PI = 180; -PI = -180; 2*PI = 360)
*/
float fixRads(float angle){
	while (angle < - M_PI)
		angle += 2*M_PI;

	while (angle > M_PI)
		angle -= 2*M_PI;

	return angle;
};

/*
	Converts from inches to centimeters
*/
inline float toCentimeters(float inches){
	return inches * 0.393700787;
}

/*
	Converts from centimeters to inches
*/
inline float toInches(float centimeters){
	return centimeters * 2.54;
}


/*
	This preprocessor logic will determine the default
	AREF voltage for the current Arduino being compiled
*/
#if defined(__AVR__)
	
	/*
		AVR family usualy is 3.3v when running on 8Mhz,
		and 5.0 if running on 16Mhz
	*/	
	#if 	F_CPU <= 8000000
		#warning "ADC_DEFAULT_AREF = 3.3v | AVR <=8Mhz clock"
		#define ADC_DEFAULT_AREF	3.3
	#elif 	F_CPU <= 16000000
		#warning "ADC_DEFAULT_AREF = 5.0v | AVR >8Mhz & <=16Mhz clock"
		#define ADC_DEFAULT_AREF	5.0
	#else
		#warning "ADC_DEFAULT_AREF = 5.0v | AVR Default AREF"
		#define ADC_DEFAULT_AREF	5.0
	#endif

#elif defined(__PIC32MX__)

	/*
		Currently, all Arduinos with PIC32 are 3.3v
	*/
	#pragma message("ADC_DEFAULT_AREF = 3.3v | PIC32 Archtecture")
	#define ADC_DEFAULT_AREF		3.3

#elif defined(__arm__)

	/*
		Currently, all Arduinos with ARM are 3.3v
	*/
	#pragma message("ADC_DEFAULT_AREF = 3.3v | ARM Archtecture")
	#define ADC_DEFAULT_AREF		3.3	

#endif

#ifndef ADC_DEFAULT_AREF
	#warning "ADC_DEFAULT_AREF = 5.0v | Default value"
	#define ADC_DEFAULT_AREF		5.0
#endif

/*
	With tests, I could notice an average voltage output
	from the regulator of (VCC * (1 - 0.03)) as the output.

	An average of 3% drop is the normal.

	So we define that to fix some small erros.

	NOTE: This feature is disabled by default.
*/
// #define ADC_VOLTAGE_PROP	0.03
#define ADC_VOLTAGE_PROP	0.00

#endif