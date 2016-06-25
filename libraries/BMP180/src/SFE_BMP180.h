/*
	SFE_BMP180.h
	Bosch BMP180 pressure sensor library for the Arduino microcontroller
	Mike Grusin, SparkFun Electronics

	Uses floating-point equations from the Weather Station Data Logger project
	http://wmrx00.sourceforge.net/
	http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf

	Forked from BMP085 library by M.Grusin

	version 1.0 2013/09/20 initial version
	Verison 1.1.2 - Updated for Arduino 1.6.4 5/2015
	Version 1.2 - Forked by igg2: https://github.com/igg2/Arduino-BMP180
	  * Added reset() method to perform a soft-reset
	  * Added setCal() and readCal() methods for more flexibility reading/writing calibration data
	  * Added reset and retry when failing to connect
	  * Added more detailed error reporting on failure to connect

	Our example code uses the "beerware" license. You can do anything
	you like with this code. No really, anything. If you find it useful,
	buy me a (root) beer someday.
*/

#ifndef SFE_BMP180_h
#define SFE_BMP180_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SFE_BMP180
{
	public:
		SFE_BMP180(); // base type

		char begin();
			// call pressure.begin() to initialize BMP180 before use
			// returns 1 if success, 0 if failure (bad component or I2C bus shorted?)
		
		char startTemperature(void);
			// command BMP180 to start a temperature measurement
			// returns (number of ms to wait) for success, 0 for fail

		char getTemperature(double &T);
			// return temperature measurement from previous startTemperature command
			// places returned value in T variable (deg C)
			// returns 1 for success, 0 for fail

		char startPressure(char oversampling);
			// command BMP180 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns (number of ms to wait) for success, 0 for fail

		char getPressure(double &P, double &T);
			// return absolute pressure measurement from previous startPressure command
			// note: requires previous temperature measurement in variable T
			// places returned value in P variable (mbar)
			// returns 1 for success, 0 for fail

		double sealevel(double P, double A);
			// convert absolute pressure to sea-level pressure (as used in weather data)
			// P: absolute pressure (mbar)
			// A: current altitude (meters)
			// returns sealevel pressure in mbar

		double altitude(double P, double P0);
			// convert absolute pressure to altitude (given baseline pressure; sea-level, runway, etc.)
			// P: absolute pressure (mbar)
			// P0: fixed baseline pressure (mbar)
			// returns signed altitude in meters

		char getError(void);
			// If any library command fails, you can retrieve an extended
			// error code using this command. Errors are from the wire library: 
			// 0 = Success
			// 1 = Data too long to fit in transmit buffer
			// 2 = Received NACK on transmit of address
			// 3 = Received NACK on transmit of data
			// 4 = Other error
			// 5 - init failed, bad id
			// 6 - init failed, bad id after bad cal read
			// 7 - init failed, bad cal read

		char reset(void);
			// Sends a soft reset command to BMP180, causing it to perform a power-on reset
			// From datasheet:
			// Soft reset (register E0): If set to 0xB6, will perform the same sequence as power on reset.
			// returns 1 for success, 0 for fail from I2C comms

		void setCal(int16_t _AC1, int16_t _AC2, int16_t _AC3, int16_t _VB1, int16_t _VB2, int16_t _MB, int16_t _MC, int16_t _MD,
			uint16_t _AC4, uint16_t _AC5, uint16_t _AC6 );
			// Sets the calibration values instead of reading them from the chip.

		char readCal(int16_t &_AC1, int16_t &_AC2, int16_t &_AC3, int16_t &_VB1, int16_t &_VB2, int16_t &_MB, int16_t &_MC, int16_t &_MD,
			uint16_t &_AC4, uint16_t &_AC5, uint16_t &_AC6 );
		char readCal();
			// Read the calibration values in the chip, then optionally set the references to them passed in as params.

	private:
	
		char readInt(char address, int16_t &value);
			// read an signed int (16 bits) from a BMP180 register
			// address: BMP180 register address
			// value: external signed int for returned value (16 bits)
			// returns 1 for success, 0 for fail, with result in value

		char readUInt(char address, uint16_t &value);
			// read an unsigned int (16 bits) from a BMP180 register
			// address: BMP180 register address
			// value: external unsigned int for returned value (16 bits)
			// returns 1 for success, 0 for fail, with result in value

		char readBytes(unsigned char *values, char length);
			// read a number of bytes from a BMP180 register
			// values: array of char with register address in first location [0]
			// length: number of bytes to read back
			// returns 1 for success, 0 for fail, with read bytes in values[] array
			
		char writeBytes(unsigned char *values, char length);
			// write a number of bytes to a BMP180 register (and consecutive subsequent registers)
			// values: array of char with register address in first location [0]
			// length: number of bytes to write
			// returns 1 for success, 0 for fail
		
		void calcParams();
			// calculates the correction parameters from the calibration data
			
		int16_t AC1,AC2,AC3,VB1,VB2,MB,MC,MD;
		uint16_t AC4,AC5,AC6; 
		double c5,c6,mc,md,x0,x1,x2,y0,y1,y2,p0,p1,p2;
		char _error;
		bool has_cal; // set to false in the constructor, this flag lets us know if calibration has been done
};

#define BMP180_ADDR 0x77 // 7-bit address

#define	BMP180_REG_CONTROL 0xF4
#define	BMP180_REG_RESULT 0xF6
#define	BMP180_REG_RESET 0xE0
#define	BMP180_REG_ID 0xD0
#define	BMP180_REG_ID_VAL 0x55

#define	BMP180_COMMAND_TEMPERATURE 0x2E
#define	BMP180_COMMAND_PRESSURE0 0x34
#define	BMP180_COMMAND_PRESSURE1 0x74
#define	BMP180_COMMAND_PRESSURE2 0xB4
#define	BMP180_COMMAND_PRESSURE3 0xF4
#define	BMP180_COMMAND_RESET 0xB6

#endif
