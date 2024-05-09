## Description
This project aims to develop an IoT template for small scale IoT project deployment by using ESP32.

## Requirements
PlatformIO

## Libraries
TFT_eSPI
XPI2046_Touchscreen

## Modifications done for TFT_eSPI/User_Setup.h file

UNCOMMENT line 88, #define TFT_WIDTH 240 <br />
UNCOMMENT line 92, #define TFT_HEIGHT 320 <br />
UNCOMMENT line 132, and changed #define TFT_BL 32 to #define TFT_BL 21 <br />
UNCOMMENT line 133, #define TFT_BACKLIGHT_ON HIGH <br />
COMMENT line 170, #define TFT_MISO PIN_D6 <br />
COMMENT line 171, #define TFT_MOSI PIN_D7 <br />
COMMENT line 172, #define TFT_SCLK PIN_D5 <br />
COMMENT line 174, #define TFT_CS PIN_D8 <br />
COMMENT line 175, #define TFT_DC PIN_D3 <br />
COMMENT line 176, #define TFT_RST PIN_D4 <br />
UNCOMMENT line 212, and changed #define TFT_MISO 19 to #define TFT_MISO 16 <br />
UNCOMMENT line 213, and changed #define TFT_MOSI 23 to #define TFT_MOSI 13 <br />
UNCOMMENT line 214, and changed #define TFT_SCLK 18 to #define TFT_SCLK 14 <br />
UNCOMMENT line 215, #define TFT_CS 15 <br />
UNCOMMENT line 216, #define TFT_DC 2 <br />
UNCOMMENT line 218, and changed #define TFT_RST -1 to #define TFT_RST 12 <br />
ADD line 220, #define TOUCH_CS 33 <br />
COMMENT line 364, #define SPI_FREQUENCY 27000000 <br />
UNCOMMENT line 366, #define SPI_FREQUENCY 55000000 <br />
UNCOMMENT line 378, #define USE_HSPI_PORT

## Modifications done for XPT2046_Touchscreen/XPT2046_Touchscreen.h
```c
#include "Arduino.h"
#include <SPI.h>
// Added the following
#if defined(__IMXRT1062__)
#if __has_include(<FlexIOSPI.h>)
	#include <FlexIOSPI.h>
#endif
#endif
// End
```
```c
// Modified class XPT2046_Touchscreen
class XPT2046_Touchscreen {
public:
	constexpr XPT2046_Touchscreen(uint8_t cspin, uint8_t tirq=255)
		: csPin(cspin), tirqPin(tirq) { }
	bool begin(SPIClass &wspi = SPI); // Added parameters for begin()
//Added the following
#if defined(__FLEXIO_SPI_H_)
  bool begin(FlexIOSPI &wflexspi);
#endif
// End

	TS_Point getPoint();
	bool tirqTouched();
	bool touched();
	void readData(uint16_t *x, uint16_t *y, uint8_t *z);
	bool bufferEmpty();
	uint8_t bufferSize() { return 1; }
	void setRotation(uint8_t n) { rotation = n % 4; }
// protected:
	volatile bool isrWake=true;

private:
	void update();
	uint8_t csPin, tirqPin, rotation=1;
	int16_t xraw=0, yraw=0, zraw=0;
	uint32_t msraw=0x80000000;
// Added the following
  SPIClass *_pspi = nullptr;
#if defined(_FLEXIO_SPI_H_)
  FlexIOSPI *_pflexspi = nullptr;
#endif
// End
};
```
## Modifications done for XPT2046_Touchscreen/XPT2046_Touchscreen.cpp
```c++
// Modified XPT_Touchscreen::begin()
bool XPT2046_Touchscreen::begin(SPIClass &wspi) // Added argument
{
	// Added the following
	_pspi = &wspi;
	_pspi->begin();
	// End
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);
	if (255 != tirqPin) {
		pinMode( tirqPin, INPUT );
		attachInterrupt(digitalPinToInterrupt(tirqPin), isrPin, FALLING);
		isrPinptr = this;
	}
	return true;
}

// Added the following
#if defined(_FLEXIO_SPI_H_)
#define FLEXSPI_SETTING     FlexIOSPISettings(2000000, MSBFIRST, SPI_MODE0)
bool XPT2046_Touchscreen::begin(FlexIOSPI &wflexspi)
{
	_pspi = nullptr; // make sure we dont use this one... 
	_pflexspi = &wflexspi;
	_pflexspi->begin();
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);
	if (255 != tirqPin) {
		pinMode( tirqPin, INPUT );
		attachInterrupt(digitalPinToInterrupt(tirqPin), isrPin, FALLING);
		isrPinptr = this;
	}
	return true;
}
#endif
// End
```
```c++
/* Modified XPT2046_Touchscreen::update()
 * Due to the significant changes made, I will not elaborate in details
*/
void XPT2046_Touchscreen::update()
{
	int16_t data[6];
	int z; // Added
	if (!isrWake) return;
	uint32_t now = millis();
	if (now - msraw < MSEC_THRESHOLD) return;
	if (_pspi) {
		_pspi->beginTransaction(SPI_SETTING);
		digitalWrite(csPin, LOW);
		_pspi->transfer(0xB1 /* Z1 */);
		int16_t z1 = _pspi->transfer16(0xC1 /* Z2 */) >> 3;
		z = z1 + 4095;
		int16_t z2 = _pspi->transfer16(0x91 /* X */) >> 3;
		z -= z2;
		if (z >= Z_THRESHOLD) {
			_pspi->transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
			data[0] = _pspi->transfer16(0xD1 /* Y */) >> 3;
			data[1] = _pspi->transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
			data[2] = _pspi->transfer16(0xD1 /* Y */) >> 3;
			data[3] = _pspi->transfer16(0x91 /* X */) >> 3;
		}
		else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
		data[4] = _pspi->transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
		data[5] = _pspi->transfer16(0) >> 3;
		digitalWrite(csPin, HIGH);
		_pspi->endTransaction();
	}	
#if defined(_FLEXIO_SPI_H_)
	else if (_pflexspi) {
		_pflexspi->beginTransaction(FLEXSPI_SETTING);
		digitalWrite(csPin, LOW);
		_pflexspi->transfer(0xB1 /* Z1 */);
		int16_t z1 = _pflexspi->transfer16(0xC1 /* Z2 */) >> 3;
		z = z1 + 4095;
		int16_t z2 = _pflexspi->transfer16(0x91 /* X */) >> 3;
		z -= z2;
		if (z >= Z_THRESHOLD) {
			_pflexspi->transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
			data[0] = _pflexspi->transfer16(0xD1 /* Y */) >> 3;
			data[1] = _pflexspi->transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
			data[2] = _pflexspi->transfer16(0xD1 /* Y */) >> 3;
			data[3] = _pflexspi->transfer16(0x91 /* X */) >> 3;
		}
		else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
		data[4] = _pflexspi->transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
		data[5] = _pflexspi->transfer16(0) >> 3;
		digitalWrite(csPin, HIGH);
		_pflexspi->endTransaction();

	}
#endif
	// If we do not have either _pspi or _pflexspi than bail. 
	else return;

	//Serial.printf("z=%d  ::  z1=%d,  z2=%d  ", z, z1, z2);
	if (z < 0) z = 0;
	if (z < Z_THRESHOLD) { //	if ( !touched ) {
		// Serial.println();
		zraw = 0;
		if (z < Z_THRESHOLD_INT) { //	if ( !touched ) {
			if (255 != tirqPin) isrWake = false;
		}
		return;
	}
	zraw = z;
// End
```
