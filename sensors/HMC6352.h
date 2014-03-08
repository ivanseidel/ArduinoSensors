/*
 	HMC6352.h - Honeywell HMC6352 Compass sensor library

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Datasheet: https://www.sparkfun.com/datasheets/Components/HMC6352.pdf

	Created by 	Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef HMC6352_h
#define HMC6352_h

#include <Arduino.h>
#include <Wire.h>
#include <Thread.h>
#include <interfaces/AngleInterface.h>

// Default I2C Address
#define HMC6352_ADDR	(0x42 >> 1)


/*
	I2C Commands
*/
#define HMC6352_CMD_WRITE_EEPROM				'w'
#define HMC6352_CMD_READ_EEPROM					'r'
#define HMC6352_CMD_WRITE_RAM					'G'
#define HMC6352_CMD_READ_RAM					'g'
#define HMC6352_CMD_ENTER_SLEEP_MODE			'S'
#define HMC6352_CMD_EXIT_SLEEP_MODE				'W'
#define HMC6352_CMD_UPDATE_BRIDGE_OFFSET		'O'
#define HMC6352_CMD_ENTER_CALIBRATION_MODE		'C'
#define HMC6352_CMD_EXIT_CALIBRATION_MODE		'E'
#define HMC6352_CMD_SAVE_OP_MODE_TO_EEPROM		'L'
#define HMC6352_CMD_GET_DATA					'A'

/*
	EEPROM Addresses
*/
#define HMC6352_EE_SLAVE_ADDRESS				0x00
#define HMC6352_EE_MAGNETOMETER_X_OFFSET_MSB	0x01
#define HMC6352_EE_MAGNETOMETER_X_OFFSET_LSB	0x02
#define HMC6352_EE_MAGNETOMETER_Y_OFFSET_MSB	0x03
#define HMC6352_EE_MAGNETOMETER_Y_OFFSET_LSB	0x04
#define HMC6352_EE_TIME_DELAY					0x05
#define HMC6352_EE_NUMBER_SUMMED_MEASUREMENTS	0x06
#define HMC6352_EE_SOFTWARE_VERSION_NUMBER		0x07
#define HMC6352_EE_OPERATION_MODE_BYTE			0x08

/*
	Operation Mode
*/

/*
	Bits 1 and 0 (Operational Mode Value) 
	Bit 1 Bit 0 Description 
	0 0 Standby Mode 
	0 1 Query Mode 
	1 0 Continuous Mode 
	1 1 Not Allowed
*/
#define HMC6352_MODE_OPERATION_MSK				(0b11 << 0)
#define HMC6352_MODE_STANDBY_MODE				(0x00 << 0)
#define HMC6352_MODE_QUERY_MODE					(0x01 << 0)
#define HMC6352_MODE_CONTINUOUS_MODE			(0x02 << 0)

// Bits 3 and 2 (Always 0)

/*
	Bit 4 (Periodic Set/Reset), 0 = Off, 1 = On
	The periodic Set/Reset function performs a re-alignment of the sensors 
	magnetic domains in case of sensor perming (magnetic upset event), operating temperature shifts, and normal thermal 
	agitation of the domains. Exposure of the HMC6352 to magnetic fields above 20 gauss (disturbing field threshold) leads to 
	possible measurement inaccuracy or “stuck” sensor readings until the set/reset function is performed. With the periodic 
	Set/Reset bit set, the set/reset function occurs every few minutes
*/
#define HMC6352_MODE_PERIODIC_SR_MSK			(0x01 << 4)

/*
	Bits 6 and 5 (Continuous Mode Measurement Rate) 
	Bit 6 Bit 5 Description 
	0 0 1 Hz Measurement Rate 
	0 1 5 Hz Measurement Rate 
	1 0 10 Hz Measurement Rate 
	1 1 20 Hz Measurement Rate
*/
#define HMC6352_MODE_RATE_MSK					(0b11 << 5)
#define HMC6352_MODE_RATE_1HZ					(0x00 << 5)
#define HMC6352_MODE_RATE_5HZ					(0x01 << 5)
#define HMC6352_MODE_RATE_10HZ					(0x02 << 5)
#define HMC6352_MODE_RATE_20HZ					(0x03 << 5)

// Bit 7 (Always 0)




class HMC6352: public Thread, public AngleInterface{

// Should be protected, but perhaps someone need this stuff...
public:
	// Cached value of the angle
	float angle;

	// Address of the I2C Compass device
	int address;

	// Continuous mode Flag
	bool inContinuousMode;

	// Buffer for internal usage
	int8_t buffer[4];

	/*
		Sends a single byte command
	*/
	void sendCommand(int8_t command){
		Wire.beginTransmission(address);
		Wire.write(command);
		Wire.endTransmission();		
	}


	/*
		Reads a single byte after command
	*/
	int8_t readByte(int8_t command){
		sendCommand(command);

		Wire.requestFrom(address, 1);
		while(Wire.available() < 1);

		return Wire.read();
	}

	/*
		Writes and reads from the EEPROM
	*/
	void writeEEPROM(int8_t eeAddress, int8_t data){
		Wire.beginTransmission(address);
		Wire.write(HMC6352_CMD_WRITE_EEPROM);
		Wire.write(eeAddress);
		Wire.write(data);
		Wire.endTransmission();

		delayMicroseconds(70);
	}
	int8_t readEEPROM(int8_t eeAddress){
		Wire.beginTransmission(address);
		Wire.write(HMC6352_CMD_READ_EEPROM);
		Wire.write(eeAddress);
		Wire.endTransmission();

		delayMicroseconds(70);

		Wire.requestFrom(address, 1);
		while(Wire.available() < 1);

		return Wire.read();
	}

public:
	/*
		Instantiate the Compass with the desired address.
		if initialize is set to false, then the Device will
		only be instantiatet, and no communication will be done
	*/
	HMC6352(int address = HMC6352_ADDR, bool initialize = true){
		angle = -10;
		this->address = address;

		if(initialize){
			Wire.begin();

			// First readings might be weird..
			readAngle();
			readAngle();

			// Cache continuousMode flag
			inContinuousMode = (getOperationMode() == HMC6352_MODE_CONTINUOUS_MODE);
		}

		Thread::Thread();
		setInterval(30);
	};

	virtual float getAngle(){
		return angle;
	}

	virtual float readAngle(){

		// Only sends command if NOT in continuous mode
		if(!inContinuousMode)
			sendCommand(HMC6352_CMD_GET_DATA);

		Wire.requestFrom(address, 2);
  
		while(Wire.available() < 2);
  
		angle = ((Wire.read() << 8) | Wire.read()) / 10.0;

		return angle;
	}

	/*
		Thread callback
	*/
	virtual void run(){
		readAngle();
		
		runned();
	}

	/*
		Get and Set current Mode Byte (RAW)
	*/
	int8_t getOperationModeRaw(){
		return readEEPROM(HMC6352_EE_OPERATION_MODE_BYTE);
	}
	void setOperationModeRaw(int8_t val){
		writeEEPROM(HMC6352_EE_OPERATION_MODE_BYTE, val);
		delayMicroseconds(120);
	}

	/*
		Get and Set Sample rate
	*/
	int8_t getSampleRate(){
		int8_t read = getOperationModeRaw();
		read = ((read >> 5) & 0b11);

		switch(read){
			case 0x00:
				return 1;
			case 0x01:
				return 5;
			case 0x02:
				return 10;
			case 0x03:
				return 20;

			default:
				return -1;
		}
	}
	void setSampleRate(int rate){
		// Filter rate
		int8_t finalRate = 0x00;

		if(rate <= 1)
			finalRate = HMC6352_MODE_RATE_1HZ;
		else if(rate <= 5)
			finalRate = HMC6352_MODE_RATE_5HZ;
		else if(rate <= 10)
			finalRate = HMC6352_MODE_RATE_10HZ;
		else
			finalRate = HMC6352_MODE_RATE_20HZ;

		// Get Mode byte
		int8_t mode = getOperationModeRaw();

		mode = (mode & (~HMC6352_MODE_RATE_MSK)) | (finalRate);

		setOperationModeRaw(mode);
	}

	/*
		Get and Set Opereation mode
	*/
	int8_t getOperationMode(){
		int8_t read = getOperationModeRaw();
		read = ((read >> 0) & HMC6352_MODE_OPERATION_MSK);

		return read;
	}
	void setOperationMode(int8_t setMode){
		// Filter rate
		int8_t finalMode = setMode & HMC6352_MODE_OPERATION_MSK;

		// Get Mode byte
		int8_t mode = getOperationModeRaw();

		mode = (mode & (~HMC6352_MODE_OPERATION_MSK)) | (finalMode);

		setOperationModeRaw(mode);

		delayMicroseconds(1000);
		inContinuousMode = (getOperationMode() == HMC6352_MODE_CONTINUOUS_MODE);
	}

	/*
		Set the default mode to be Continuous
	*/
	void setContinuousMode(){
		setOperationMode(HMC6352_MODE_CONTINUOUS_MODE);
	}
	
	/*
		Set the default mode to be Standby
	*/
	void setStandbyMode(){
		setOperationMode(HMC6352_MODE_STANDBY_MODE);
	}

	/*
		Set the default mode to be Query
	*/
	void setQueryMode(){
		setOperationMode(HMC6352_MODE_QUERY_MODE);
	}

	/*
		Periodic Set/Reset mode
	*/
	bool isAutoFixEnabled(){
		int8_t mode = getOperationModeRaw();

		return (mode & HMC6352_MODE_PERIODIC_SR_MSK);
	}
	void setAutoFixEnabled(bool enabled){
		// Get Mode byte
		int8_t mode = getOperationModeRaw();

		mode = (mode & (~HMC6352_MODE_PERIODIC_SR_MSK)) | (enabled * HMC6352_MODE_PERIODIC_SR_MSK);

		setOperationModeRaw(mode);
	}

	/*
		Enter sleep mode (standby)
	*/
	void sleep(){
		sendCommand(HMC6352_CMD_ENTER_SLEEP_MODE);
		delayMicroseconds(10);
	}

	/*
		Wake up the device
	*/
	void wake(){
		sendCommand(HMC6352_CMD_EXIT_SLEEP_MODE);
		delayMicroseconds(100);
	}

	/*
		Save the current mode as default (Sleep or Wake)
	*/
	void saveMode(){
		sendCommand(HMC6352_CMD_SAVE_OP_MODE_TO_EEPROM);
	}

	/*
		Calibration mode.

		Recommended time: 6 seconds up to 3 minutes.
		Optimal: 2 rotations in 20 seconds.
		(Remember to FINISH calibration)
	*/
	void startCalibration(){
		sendCommand(HMC6352_CMD_ENTER_CALIBRATION_MODE);
	}
	void finishCalibration(){
		sendCommand(HMC6352_CMD_EXIT_CALIBRATION_MODE);
	}

	/*
		Facilitate calibration.
		call this method with the desired duration (in Ms)
		and the Compass will enter and exit both states after
		the desired duration.
	*/
	void calibrate(long durationMs = 20000){
		startCalibration();

		// We use Microseconds to allow calibration on Interruptions
		delayMicroseconds(durationMs*1000);

		finishCalibration();
	}

	/*
		Get version
	*/
	int8_t getVersion(){
		return readEEPROM(HMC6352_EE_SOFTWARE_VERSION_NUMBER);
	}

	
	/*
		Get and Set address
	*/
	int8_t getAddress(){
		return address << 1;
	}

	void setAddress(int8_t newAddress){
		writeEEPROM(HMC6352_EE_SLAVE_ADDRESS, newAddress);

		// Also change the local address (otherwise we will lost the device...)
		address = (newAddress >> 1);

		delayMicroseconds(300);
	}

	/*
		========== SANDBOX ==========
		In development:
			Search devices reliably
	*/

	/*
		This will return the same address of the local variable,
		however is usefull for checking if the device exist over
		the I2C wiring.

		timeoutMs is the maximum time waiting for a response.
	*/
	/*int8_t getAddressDumb(long timeoutMs){
		Wire.beginTransmission(address);
		Wire.write(HMC6352_CMD_READ_EEPROM);
		Wire.write(HMC6352_EE_SOFTWARE_VERSION_NUMBER);
		Wire.endTransmission();

		delayMicroseconds(70);

		Wire.requestFrom(address, 1);

		long start = micros();
		while(Wire.available() < 1)
			if(micros() - start > timeoutMs*1000)
				return 0;

		return Wire.read();
	}*/

	/*
		This method will return the first HMC6352 over the I2C bus,
		with the address closest and greater than startAddress.

		If none is found, false is returned;
	*/

	/*static int findDevice(int startAddress = 1){
		HMC6352 searchDevice(startAddress, false);

		for(int8_t i = startAddress; i < 0xFF; i++){
			searchDevice.address = (i >> 1);

			Serial.print(i);
			Serial.print("\t");
			int8_t foundAdr = searchDevice.getAddressDumb(1);
			Serial.println(foundAdr);
			if(foundAdr == i){
				Serial.print("Found: ");
				Serial.println(i);
				return i;
			}
		}

		Serial.println("NOT FOUND");

		return false;
	}*/

};

#endif