/*
 	MLX90614.h - Reliable Library that controlls MLX90614 Temperature sensors

	For instructions, go to https://github.com/ivanseidel/Arduino-Sensors

	Created by 	Ivan Seidel Gomes, June, 2013.
	Released into the public domain.
*/

#ifndef MLX90614_h
#define MLX90614_h

#include <Arduino.h>

#include <Thread.h>
#include <interfaces/TemperatureInterface.h>

#include <inttypes.h>
#include <I2Cmaster.h>

// I2C bits to read and write
#define I2C_READ 						0x1
#define I2C_WRITE 						0x0

/*
	MLX90614 Memory addresses
*/
#define MLX90614_ADR_SET_ADDRESS		0x2E
#define MLX90614_ADR_GET_TEMP1			0x07

/*
	Table used to do CRC8 Algorithm, to change Address
*/
const unsigned char crc_table[] = {
	0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,
	0x24,0x23,0x2A,0x2D,0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,
	0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,0xE0,0xE7,0xEE,0xE9,
	0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
	0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,
	0xB4,0xB3,0xBA,0xBD,0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,
	0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,0xB7,0xB0,0xB9,0xBE,
	0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
	0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,
	0x03,0x04,0x0D,0x0A,0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,
	0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,0x89,0x8E,0x87,0x80,
	0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
	0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,
	0xDD,0xDA,0xD3,0xD4,0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,
	0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,0x19,0x1E,0x17,0x10,
	0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
	0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,
	0x6A,0x6D,0x64,0x63,0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,
	0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,0xAE,0xA9,0xA0,0xA7,
	0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
	0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,
	0xFA,0xFD,0xF4,0xF3
};

class MLX90614: public Thread, public TemperatureInterface{
protected:
	/*
		This method is used when changing the address of the MLX90614
	*/
	static char CRC8(unsigned char buffer[], unsigned int len){
		unsigned char m_crc = 0;
		int m_byte_count = 0;

		for(int i = 0; i < len; i++) {
			m_byte_count++;
			m_crc = crc_table[ m_crc ^ buffer[i] ];
		}

		return m_crc;
	}

	/*
		Keeps the current address of the MLX90614
	*/
	int adr;

	/*
		Cached value of the temperature
	*/
	float temperature;

	// Flag that indicates if the bus is initialized
	static bool busInitialized;

public:
	/*
		Construct the MLX90614 object with the given address
	*/
	MLX90614(int _adr){
		setAddress(_adr);

		if(!MLX90614::busInitialized){
			MLX90614::init();
			MLX90614::busInitialized = true;
		}

		Thread::Thread();
		setInterval(20);
	}

	/*
		Initialize I2C bus and enable pull-ups
	*/
	static void init(){
	  	// Wire.begin();
		i2c_init(); //Initialise the i2c bus
		PORTC = (1 << PORTC4) | (1 << PORTC5);//enable pullups
	}

	/*
		Set the current object address to the given one.

		NOTE: It doesen't actually change the device Address.
		See 'changeAddress' for that purpose
	*/
	void setAddress(int _adr){
		adr = _adr;
	}

	/*
		Return the current device address
	*/
	int getAddress(){
		return adr;
	}

	/*
		Put the desired device to sleep
	*/
	bool sleep(){
		if(!i2c_start_wait(adr<<1 + I2C_WRITE))
			return false;

		i2c_write(0xFF);
		i2c_write(0xE8);
		i2c_stop();

		return true;
	}

	/*
		Wake ALL MLX90614 devices on the I2C bus
	*/
	void wake(){
		digitalWrite(SDA, LOW);
		delayMicroseconds(36000);
		digitalWrite(SDA, HIGH);
	}

	/*
		Comunicate with the sensor, sample the Temperature
		and save to the Cached value
	*/
	virtual float readTemperature(){

		int8_t _adr = adr << 1;
		int data_low = 0;
		int data_high = 0;
		int pec = 0;
		
		i2c_start_wait(_adr | I2C_WRITE);
		i2c_write(MLX90614_ADR_GET_TEMP1);
		
	    // read
		i2c_rep_start(_adr | I2C_READ);
		data_low = i2c_readAck(); //Read 1 byte and then send ack
		data_high = i2c_readAck(); //Read 1 byte and then send ack
		pec = i2c_readNak();
		i2c_stop();
		
	    /*
	    	This converts high and low bytes together and processes temperature,
	    	MSB is a error bit and is ignored for temps
	    */
	    // 0.02 degrees per LSB (measurement resolution of the MLX90614)
		double tempFactor = 0.02; 

		// Zero the data
		double tempData = 0x0000; 

		// Data past the decimal point
		int frac; 
		
	    /*
	    	This masks off the error bit of the high byte,
	    	then moves it left 8 bits and adds the low byte.
    	*/
		
		tempData = (double)(((data_high & 0x007F) << 8) + data_low);
		tempData = (tempData * tempFactor)-0.01;
		
		float celcius = tempData - 273.15;

		/*
			If you are using ATMEGA328 or like processors,
			the following line end's up with I2C failures.
		*/
		TWCR = 0;

		temperature = celcius;

		return getTemperature();
	}

	/*
		Returns the cached value of the temperature
	*/
	virtual float getTemperature(){
		return temperature;
	}

	/*
		Thread callback
	*/
	virtual void run(){
		readTemperature();
		
		runned();
	}

	/*
		Change the address of the MLX90614 over the I2C.

		Procedure:
			1.: Remove ALL devices from the BUS and let only the
				desired MLX90614 to change address

			2.: Write a code that calls changeAddress(addr)
				once and stops.

			3.: Power off and on the MLX90614.

			4.: Done. Have fun!
	*/
	bool changeAddress(int newAdr){
	    // Erase Address memory FIRST
	    if(newAdr !=  0x00)
			changeAddress(0x00);
		
	    // Prepare 3 byte message
		byte msg[4] = {MLX90614_ADR_SET_ADDRESS, newAdr, 0x00,0x00};
		msg[3] = CRC8(msg, 3);

		if(!i2c_start_wait(0 + I2C_WRITE))
			return false;

		i2c_write(msg[0]);
		i2c_write(msg[1]);
		i2c_write(msg[2]);
		
		if(i2c_write(msg[3])){
			i2c_stop();
		}else{
			i2c_stop();
			return false;
		}

		if(newAdr !=  0x00)
			sleep();
		
		// DelayMicroseconds can be used within Interruptions
		delayMicroseconds(50000);

		if(newAdr !=  0x00)
			wake();

	    adr = newAdr;

		return true;
	}

	/*
		This method will return the first connected and working device
		address between 'start' and 'end'
	*/
	static int scan(int start = 0, int end = 127){
		bool find;
		int i;

		for(i = start; i <= end; i++){
			find = i2c_start_wait(i<<1 | I2C_WRITE);
			i2c_stop();

			if(find)
				return i;

			TWCR = 0;
		}
		return -1;
	}

	/*
		This method will return a list of connected MLX90614 addresses
		connected and working, between 'start' and 'end', with a limit
		of 'maxSize' devices found. All devices will be stored in the 
		'addresses' buffer (IT SHOULD STORE AT LEAST 'maxSize' int elements).

		The total number of devices found will be returned.
	*/
	static int scanAll(int start, int end, int maxSize, int* addresses){
		int total;
		int s;
		total = 0;
		for(int x = start; x < end && total < maxSize; x++){
			s = scan(x);
			if(s==-1){
				break;
			}else{
				addresses[total] = s;
				x = s;
				total++;
			}
		}
		return total;
	}
};

// Static atributes initialization
bool MLX90614::busInitialized = false;

#endif
