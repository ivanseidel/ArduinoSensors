/*
 	MPUSensor.h - Reliable Library that controlls both MPU9150 and MPU6050

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef IMU_h
#define IMU_h

#include <Arduino.h>
#include <inttypes.h>

#include <Wire.h>
#include "I2Cdev.h"
#include "sensors/MPU/dmpMPU9150.h"
#include <Thread.h>
#include <interfaces/AngleInterface.h>

class MPUSensor: public Thread, public AngleInterface{

protected:
	uint8_t _fifoBuffer[64];
	uint8_t _packetSize;
	uint8_t _fifoCount;

	// uint8_t teapotPacket[14];

public:

	//int16_t ax, ay, az, gx, gy, gz, mx, my, mz;

	// Flag that indicates if there is new data available
	bool hasNewData;

	/*
		Those flags can be manipulated.
		When enabled, the corresponding measurement will
		be always refreshed whenever a new sample is read
		from the MPU.
	*/
	// Raw Gravity
	bool readGravity;
	// Raw data
	bool readAccel;
	// Linear Acelleration (Gravity Free)
	bool readAccelReal;
	// World-frame Acelleration (Gravity Free)	
	bool readAccelWorld;
	// Yaw, Pitch and Roll readings
	bool readYPR;

	// Return types (corresponds to the flags above)
	Quaternion quaternion;
	VectorInt16 accel;
	VectorInt16 accelReal;
	VectorInt16 accelWorld;
	VectorFloat gravity;
	float ypr[3]; // In RADIANS

	// The MPU Device
	MPU9150 device;

	MPUSensor(TwoWire TWI){
		// Initialize local flags
		setDefaults();
		// Public flag, to indicate if there is new data proccessed
		hasNewData = false;
		// Set current TWI on I2Cdev
		I2Cdev::Wire = TWI;
		TWI.begin();
		// Instantiate device
		device = MPU9150();
		
		//ax = ay = az = 0;
		//gx = gy = gz = 0;
		//mx = my = mz = 0;

		// teapotPacket = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
	};

	/*
		This will initialize the DMP and make the MPU ready to begin.

		the return of this function indicates if it sucessfully
		loaded the DMP and the device is ready to start sampling.

		true: OK | false: FAIL
	*/
	virtual bool initialize(){

		// Disable DMP
		device.resetDMP();
//		delay(20);
		device.setDMPEnabled(false);
//		delay(20);
		// Reset IMU
    	device.reset();
//		delay(20);
    	// Initialize
    	device.initialize();
//		delay(20);
    	// Test connection
    	// if(!device.testConnection()) Serial.println("FAIL CONNECT");
    	// if(!device.testConnection()) return false;
    	// Configure DMP (0 = Success)
    	if(device.dmpInitialize() != 0) return false;
    	// Read the returning packet size
    	_packetSize = device.dmpGetFIFOPacketSize();
    	Serial.println(_packetSize);

    	return true;
	}

	/*
		Whenever you want to start samping, call this function
	*/
	virtual void start(){
		// Enable DMP
    	device.setDMPEnabled(false);
		delay(100);
    	device.setDMPEnabled(true);
	}

	/*
		When you are done, stop the DMP
	*/
	virtual void stop(){
		// Disable DMP
    	device.setDMPEnabled(false);
	}

	// Return flags to false (no reading will be done)
	virtual void setDefaults(){
		readGravity = false;
		readAccel = false;
		readAccelReal = false;
		readAccelWorld = false;
		readYPR = true;
	}

	/*void update9() {
		device.getMotion9(&ax, &ay, &az, 
							  &gx, &gy, &gz, 
							  &mx, &my, &mz);
	}*/

	/*
		This Method is responsable for syncronizing the MPU.

		You should call this CONSTANTLY. Consider inserting it
		inside a Timer, and calling it from there.

		Also, This class extends Thread, witch means that it can
		be registered inside a ThreadController. Configure the
		sampling time with setInterval();

		Whenever a new sample is fresh, hasNewData will go true.
	*/
	virtual void run(){
		if (_fifoCount = device.getFIFOCount() >= _packetSize){
			// Serial.println("\tRead");

            // read a packet from FIFO
            device.getFIFOBytes(_fifoBuffer, _packetSize);
            device.resetFIFO();

            // Base data to be proccessed
            device.dmpGetQuaternion(&quaternion, _fifoBuffer);

            // Process Accel
            if(readAccel || readAccelWorld || readAccelReal)
            	device.dmpGetAccel(&accel, _fifoBuffer);

            // Process Gravity
            if(readGravity || readYPR || readAccelReal || readAccelWorld)
            	device.dmpGetGravity(&gravity, &quaternion);

            // Process YPR (IN RADIANS)
            if(readYPR)
            	device.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);

            if(readAccelReal)
            	device.dmpGetLinearAccel(&accelReal, &accel, &gravity);

            if(readAccelWorld)
            	device.dmpGetLinearAccelInWorld(&accelWorld, &accelReal, &quaternion);
			

            // Set flag that new data has come
            hasNewData = true;

            // This is the default DEMO from Invensense
            /*teapotPacket[2] = _fifoBuffer[0];
            teapotPacket[3] = _fifoBuffer[1];
            teapotPacket[4] = _fifoBuffer[4];
            teapotPacket[5] = _fifoBuffer[5];
            teapotPacket[6] = _fifoBuffer[8];
            teapotPacket[7] = _fifoBuffer[9];
            teapotPacket[8] = _fifoBuffer[12];
            teapotPacket[9] = _fifoBuffer[13];
            Serial.write(teapotPacket, 14);
            teapotPacket[11]++;*/
            runned();

        }
	};

	/*
		Returns the Yaw value of the MPU
	*/
	virtual float getAngle(){
		// Default angle returned is Yaw
		// TODO: Return selected angle
		return ypr[0];
	}

	/*
		"Tries" to read and return the fresh value of the MPU
	*/
	virtual float readValue(){
		// Run self Thread and tries to update
		run();

		return getAngle();
	}

};

#endif