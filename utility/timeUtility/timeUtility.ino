/*timeUtility

  Utility to set PCF8523 RTC to System Time.
  Current version 0.0.3-alpha by Chris Frishkorn.

  Version Release History
  -----------------------
  April 15th, 2017    - v0.0.3-alpha - Updated to work with new SD Shield and RTC.
  May 15th, 2016      - v0.0.2-alpha - Added LCD output (issue #84).
  December 28th, 2015 - v0.0.1-alpha - Utility created.
*/

#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include <Adafruit_RGBLCDShield.h>

#define PAUSE 2500

RTC_PCF8523 RTCA;
Adafruit_RGBLCDShield LCD = Adafruit_RGBLCDShield();

// Error sub-routine.
void error(char *str) {
  Serial.print("Error: ");
  Serial.println(str);
  while (1);
}

void setup() {
  // Intialize Serial RTC, and I2C.
  Serial.begin(57600);
  Wire.begin();
  LCD.begin(16, 2);
  LCD.clear();
  LCD.setBacklight(7);
  LCD.setCursor(0, 0);
  LCD.print("Initializing RTC");
  RTCA.begin();
  if (!RTCA.initialized()) {
    Serial.println("RTC is NOT running!");
  }

  // Set RTC time and date.
  RTCA.adjust(DateTime(__DATE__, __TIME__));
  DateTime now = RTCA.now();
  Serial.print("Date and Time set to: ");
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(", ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  LCD.setCursor(0, 1);
  LCD.print("RTC Set!");
  delay(PAUSE);
  Serial.println("Done...");
  Serial.println("RTC Set Please Install Firmware");
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("RTC Set Please");
  LCD.setCursor(0, 1);
  LCD.print("Install Firmware");
}

void loop() {
  // File is for running once, not continous operation.
}
