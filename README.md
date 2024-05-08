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