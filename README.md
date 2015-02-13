# Arduino-Sensors

A "Library" of Libraries of Sensors. Infrared, Ultrasonic, Compass and many others,
ready to work with [ArduinoThread](https://github.com/ivanseidel/ArduinoThread) and fully object oriented.

This library was made to provide lot's of out-of-the-box features, such as 'Interfaces', 'Threadable' sensors,
and reliability over C++ Objects.

If you are a developer, please consider developing more sensor Libraries following the given pattern. I will be
pleased to incorporate it to the library!

## Installation

1. [Download](https://github.com/ivanseidel/ArduinoSensors/archive/master.zip) the Latest release from gitHub.
2. Unzip and modify the Folder name to "ArduinoSensors" (Remove the '-version')
3. Paste the modified folder on your Library folder (On your `Libraries` folder inside Sketchbooks or Arduino software).
4. Download and Install [ArduinoThread](https://github.com/ivanseidel/ArduinoThread)
5. Restart the Arduino IDE

## Included Generic Classes:

* AnalogIn
* DigitalIn
* DigitalOut
* AnalogVoltage

## Included Sensor Classes:

* MLX90614 Temperature Sensor
* HMC6352 Compass sensor
* EZ Ultrasonic Sensor
* Ping Ultrasonic Sensor
* HC-SR04 Ultrasonic Sensor
* InvenSense IMU MPU9150
* InvenSense IMU MPU6050
* Sharp Infrared Short Distance Sensor (GP2D120XJ00F)
* Sharp Infrared Medium Distance Sensor (GP2Y0A21YK)
* Sharp Infrared Long Distance Sensor (GP2Y0A02YK0F)

# The Concept of this Library

If you are here, you are probably looking for some sensor library. Perhaps an Distance sensor, or a Compass sensor, or even a IMU sensor. What do they all have in common? They are all sensors, and do some measurement. Perhaps an Angle, or a Distance, or a Speed...

Some libraries uses functions and others use Classes witch are better to maitain and even to understand the code. This Library tries to create a patterns that allows others to create better Sensor libraries with it, while keeping method names generic and compatibility across multiple platforms. Also, Sensors extend from it's base the a Thread (from [ArduinoThread](https://github.com/ivanseidel/ArduinoThread) Library), allowing `async` fetch of sensor data pretty easily.

But to do all that, we must have a strong and nice Base to work with. For example: There are many platforms that are 3.3v, 5v and even 1.8V. How can we make it work with multiple ADC values? How can we make it simple to develop and to use? Continue reading and you will understand.

# Base Classes

Consists of a set of classes that helps another classes like `DigitalIn`, `AnalogIn`, `DigitalOut`, `AnalogVoltage`. 

Also contains a set of Interfaces such as `AngleInterface`, `DistanceInterface`, `TemperatureInterface`, `VoltageInterface`, used by different sensor types.

Let's say we have a new Temperature sensor, and we want to create a Library for it. What we expect to have:
* **Generic method names.** In this case, it would extend a `TemperatureInterface`, that has the method `getTemperature` and `readTemperature`
* **Caching it's value.** We should be able to both `read` it's value (actually fetch sensor data, read analog, communicate...), or `get` it's current value (basically, return what was read last, like a caching).
* **Work Asynchronously.** Implementing Thread from [ArduinoThread](https://github.com/ivanseidel/ArduinoThread) Library, we can make it read with a fixed timing, and  get it's value latter without further delay.

## Interfaces

Interfaces are a very good way of transforming your usual code into a more generic way. It's basicaly a set of methods and properties (variables), that a Class will inherit. For example, every class that extends `AngleInterface`, will have a method called `readAngle` and `getAngle`. If a new sensor library is created, that reads an angle (perhaps a potentiometer, Compass or even a encoder), it could inherit from this interface, and get those methods.

I will explain why it's good to use with an experience I had:

I was developing a complex robot, that would require multiple Distance Sensor types. That included two types of Infrared distance sensor, and one type of Ultrasonic sensor. I have developed this library thinking that whenever I needed, I could change the sensor in hardware for another, and it would still work. Shure that there are physical differences between them, but what I wanted was to work with multiple sensors without altering more than ONE line of code. I so I did it. We could test with different sensors, without taking so much time on software and also hardware.

We also used a IMU from Invensense, that has a Compass. After a while, we decide to change that for a simple Compass Sensor. Both of the sensors libraries was made by me, implementing it's `AngleInterface`.

## Base Classes

Base Classes are used by other classes, and also by you (if you want), providing a even more High Level approach for IO manipulation.

### DigitalIn Class

Provides a easy approach for manipulating a Digital Input pin:

```
DigitalIn myButton(9);

// Check if Pin is HIGH
if (myButton)...
  
// Check if Pin is LOW
if (myButton == LOW)...

// Reads it's value
boolean val = myButton.readValue();

// Get last read value
val = myButton.getValue();
```

Also, if you initialize it passing `true` as a seccond param, every read will be inverted:
```
DigitalIn myInvertedButton(10, true);

// Check if Pin is LOW
if(myInvertedButton)

// Read it's value (inverted)
myInvertedButton.readValue();
```

### DigitalOut Class

Used to manipulate Digital Output pins in a easy way.

```
DigitalOut myLed(13);

// Turns LED on
myLed = HIGH;
myLed.turn(HIGH);
myLed.turnOn();

// Turns LED off
myLed = LOW;
myLed.turn(LOW);
myLed.turnOff();
```

Like DigitalIn, if you assign an seccond parameter in it's initialization, it will automatically invert it's output
```
DigitalOut myLedInverted(13, true);

// Turns LED off
myLedInverted = HIGH

// You can also check it's istate like this:
if(myLedInverted == HIGH) ...
```

### AnalogIn Class

We usually need to use Analog Pins as Digital. Or even Read it's value multiple times. I usually consider a lot the timing of things, knowing every milisecond that is happening on my program. Because of that you might consider using this class instead of usual `analogRead` function.

```
AnalogIn myPot(A0);

// Reads it's value
val = myPot.read();

// Get it's cached value (does not reads ADC, only returns last value)
val = myPot.getValue();
```

One usual case, is setting a threshold to compare. We do it for you:

```
// Set threshold for 500
myPot.setThreshold(500);

// Check if below 500
if(myPot.belowThreshold())...

if(myPot == false)...

// Check if above 500
if(myPot.aboveThreshold())...

if(myPot == true)...
```

## AnalogVoltage Class

This is a nice step for next generations of Libraries, because there are many Analog Sensors around, that it's converted value should be mapped to a Voltage, not a ADC value. But that's a problem becoming larger today, since there are platforms that runs with 5v, others with 3.3v... And imagine, every single library thrying to work whit that (or not)?

```
AnalogVoltage myVoltimeter(A0);

// Read the voltage (in Volts)
float pinVoltage = myVoltimeter.readVoltage();
```

Well, you might be wondering: How does it do it's magic? The answer is pretty simple: **Preprocessor macros**.

In compiling time, we detect weather you are using a 3.3v or 5v platform, and set a definition called `ADC_DEFAULT_AREF`. We use it later to scale the ADC to a voltage. But keep calm, you can override that pretty easily, and allows you to create custom `linear` voltage in readers (Like a voltage divider, to read a battery voltage).

```
// 0 in ADC now means -5v. full ADC (1024) means 10v
AnalogVoltage myCustomVoltimeter(A0, -5, 10);

float pinVoltage = myCustomVoltimeter.readVoltage();
```

# Working toguether with ArduinoThread

Read more about [ArduinoThread](https://github.com/ivanseidel/ArduinoThread) here. It's a library I created to simplify complex softwares that requires multiple async tasks. Let's say:
* Read Compass every 50ms
* Read Distance Sensors every 70ms
* Read a button every 5ms
* Get battery voltage every 1 minute
* and so on...

If you want a REAL example of it working, take a look at our [2013 RoboCup Junior Soccer Robot code](https://github.com/ivanseidel/Robot-Soccer-2013/tree/master/Software/Soccer_v02).

The idea is that you can register Threads in ThreadController (a group of Threads), and let the system check if it should be runned. In that way, you just GET sensor values in your main code, without consuming time every time you need something.

```
// Create a new ThreadController that will run always
// (means it will check if it's threads should be runned everytime)
ThreadController threads(0);

// Create some sensors
DistanceInterface *myDist1 = new SharpLong(A0);
DistanceInterface *myDist2 = new SharpShort(A1);
TemperatureInterface *thermometer = new MLX90614(0x32);
AngleInterface *myCompass = new HMC6352();

// Each sensor has it's interval set to minimum possible
// or something good to work with (20ms - 70ms).

// But they ARE Thread objects, so you can make use of
// it's methods:

// Compass should be read every 60ms
myCompass.setInterval(60);

// Add to the thread controller
threads.add(&myDist1);
threads.add(&myDist2);
threads.add(&thermometer);
threads.add(&myCompass);

// Now, just call threads.run(); to automatically run what is needed
// (perhaps in a Timer Callback? or in Loop.. whatever)
loop(){
  threads.run();
  
  // Get the most recend value WITHOUT reading the sensor
  double currentAngle = myCompass->getAngle();
  float distance = myDist1->getDistance();
  [...]
}

```

## Rest of Documentation under development...
