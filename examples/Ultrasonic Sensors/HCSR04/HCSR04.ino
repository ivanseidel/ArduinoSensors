#include <Thread.h>
#include <ArduinoSensors.h>
#include <sensors/PingUltrasonic.h>

/*
	Echo and Trigger pins must be the same
	to your connections on the arduino
*/
int echoPin 	= 12;
int triggerPin 	= 13;

PingUltrasonic myUltrasonic(triggerPin, echoPin);

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
