#include <ArduinoSensors.h>
#include <sensors/PingUltrasonic.h>


PingUltrasonic myUltrasonic(echoPin, triggerPin);

void setup() {
	Serial.begin(9600);
	Serial.println("I'm alive!")
}

void loop()
{
	float distance = myUltrasonic.readDistance();

	Serial.print("Distance: ");
	Serial.print(distance);
	Serial.println("cm");

	delay(100);
}




