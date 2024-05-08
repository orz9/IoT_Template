## Description
This project aims to develop an IoT template for small scale IoT project deployment by using ESP32.

## Requirements
PlatformIO

## Libraries
TFT_eSPI
XPI2046_Touchscreen

## Modifications done for TFT_eSPI/User_Setup.h file

UNCOMMENT line 88, #define TFT_WIDTH 240
UNCOMMENT line 92, #define TFT_HEIGHT 320
UNCOMMENT line 132, and changed #define TFT_BL 32 to #define TFT_BL 21
UNCOMMENT line 133, #define TFT_BACKLIGHT_ON HIGH
COMMENT line 170, #define TFT_MISO PIN_D6
COMMENT line 171, #define TFT_MOSI PIN_D7
COMMENT line 172, #define TFT_SCLK PIN_D5
COMMENT line 174, #define TFT_CS PIN_D8
COMMENT line 175, #define TFT_DC PIN_D3
COMMENT line 176, #define TFT_RST PIN_D4
UNCOMMENT line 212, and changed #define TFT_MISO 19 to #define TFT_MISO 16
UNCOMMENT line 213, and changed #define TFT_MOSI 23 to #define TFT_MOSI 13
UNCOMMENT line 214, and changed #define TFT_SCLK 18 to #define TFT_SCLK 14
UNCOMMENT line 215, #define TFT_CS 15
UNCOMMENT line 216, #define TFT_DC 2
UNCOMMENT line 218, and changed #define TFT_RST -1 to #define TFT_RST 12
ADD line 220, #define TOUCH_CS 33
COMMENT line 364, #define SPI_FREQUENCY 27000000
UNCOMMENT line 366, #define SPI_FREQUENCY 55000000
UNCOMMENT line 378, #define USE_HSPI_PORT