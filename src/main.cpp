#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include "config.h"
#include "functions.h"

TFT_eSPI tft = TFT_eSPI();

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

// Stores current button state
bool buttonState = false;

void setup() {
  Serial.begin(115200);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(1);

  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  // Draw button 
  drawGreenButton();

  pinMode(CYD_LED_GREEN, OUTPUT);
  digitalWrite(CYD_LED_GREEN, LOW);
}

void loop() {
  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z) info on the TFT display and Serial Monitor
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    printTouchToSerial(x, y, z);

    if (buttonState) {
      Serial.println("ON");
      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > (REDBUTTON_Y)) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red button pressed");
          drawRedButton();
          digitalWrite(CYD_LED_GREEN, HIGH);
        }
      }
    }
    else {
      Serial.println("OFF");
      if ((x > (GREENBUTTON_X)) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > (GREENBUTTON_Y)) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green button pressed");
          drawGreenButton();
          digitalWrite(CYD_LED_GREEN, LOW);
        }
      }
    }
  }
}