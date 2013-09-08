#include <Thread.h>
#include <ArduinoSensors.h>
#include <sensors/PingUltrasonic.h>

/*
	Signal pin must be the same
	to your connection on the arduino
*/
int sigPin = 12;

PingUltrasonic myUltrasonic(sigPin);

void setup() {
	Serial.begin(9600);
	Serial.println("I'm alive!");
}

void loop()
{
	float distance = myUltrasonic.readDistance();

	Serial.print("Distance: ");
	Serial.print(distance);
	Serial.println("cm");

	delay(100);
}