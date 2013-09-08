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
	float x;
	while (angle < -180.0)
		angle += 360.0;

	while (x > 180.0)
		angle -= 360.0;

	return angle;
};


/*
	This is the same as the above, but working with Radians.
	(PI = 180; -PI = -180; 2*PI = 360)
*/
float fixRads(float angle){
	float x;
	while (angle < - M_PI)
		angle += 2*M_PI;

	while (x > M_PI)
		angle -= 2*M_PI;

	return angle;
};

#endif