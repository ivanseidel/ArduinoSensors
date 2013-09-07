#ifndef IMU_h
#define IMU_h

#include "Arduino.h"
#include "inttypes.h"

#include "Wire.h"
#include "I2Cdev.h"
#include "dmpMPU9150.h"
#include "Thread.h"

class MPUSensor: public Thread{

protected:
	uint8_t _fifoBuffer[64];
	uint8_t _packetSize;
	uint8_t _fifoCount;

	// uint8_t teapotPacket[14];

public:

	//int16_t ax, ay, az, gx, gy, gz, mx, my, mz;

	// Flag that indicates if there is new data available
	bool hasNewData;

	bool readGravity;
	bool readAccel;			// Raw data
	bool readAccelReal;		// Linear Acelleration (Gravity Free)
	bool readAccelWorld;	// World-frame Acelleration (Gravity Free)
	bool readYPR;

	// Return types
	Quaternion quaternion;
	VectorInt16 accel;
	VectorInt16 accelReal;
	VectorInt16 accelWorld;
	VectorFloat gravity;
	float ypr[3];	// In RADIANS

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

	bool initialize(){

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

	void start(){
		// Enable DMP
		Serial.println("starting");
    	device.setDMPEnabled(false);
		delay(100);
    	device.setDMPEnabled(true);
		Serial.println("started");
	}

	void stop(){
		// Disable DMP
    	device.setDMPEnabled(false);
	}

	// Return flags to false (no reading will be done)
	void setDefaults(){
		readGravity = false;
		readAccel = false;
		readAccelReal = false;
		readAccelWorld = false;
		readYPR = true;
	}
	/*
	void update9() {
		device.getMotion9(&ax, &ay, &az, 
							  &gx, &gy, &gz, 
							  &mx, &my, &mz);
	}
*/
	void run(){
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
		// Serial.print("\tCheck: ");
		// Serial.println(_fifoCount);
	};


};

#endif