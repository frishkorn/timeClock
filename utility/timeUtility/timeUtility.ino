/*timeUtility

  Utility to set PCF8523 RTC to System Time.
  Current version 0.0.4-beta by Chris Frishkorn.

  Version Release History
  -----------------------
  November 23rd, 2019 - v0.0.4-beta  - Changed serial output port (issue #139).
  April 15th, 2017    - v0.0.3-alpha - Updated to work with new SD Shield and RTC.
  May 15th, 2016      - v0.0.2-alpha - Added LCD output (issue #84).
  December 28th, 2015 - v0.0.1-alpha - Utility created.

  - See GitHub for complete version tracking notes.
  
*/

#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
#include <Adafruit_RGBLCDShield.h>

#define PAUSE 2500

RTC_PCF8523 RTCA;
Adafruit_RGBLCDShield LCD = Adafruit_RGBLCDShield();

// Error sub-routine.
void error(char *str) {
  SerialUSB.print("Error: ");
  SerialUSB.println(str);
  while (1);
}

void setup() {
  // Intialize Serial RTC, and I2C.
  SerialUSB.begin(57600);
  Wire.begin();
  LCD.begin(16, 2);
  LCD.clear();
  LCD.setBacklight(7);
  LCD.setCursor(0, 0);
  LCD.print("Initializing RTC");
  RTCA.begin();
  if (!RTCA.initialized()) {
    SerialUSB.println("RTC is NOT running!");
  }

  // Set RTC time and date.
  RTCA.adjust(DateTime(__DATE__, __TIME__));
  DateTime now = RTCA.now();
  SerialUSB.print("Date and Time set to: ");
  SerialUSB.print(now.month(), DEC);
  SerialUSB.print('/');
  SerialUSB.print(now.day(), DEC);
  SerialUSB.print('/');
  SerialUSB.print(now.year(), DEC);
  SerialUSB.print(", ");
  SerialUSB.print(now.hour(), DEC);
  SerialUSB.print(':');
  SerialUSB.print(now.minute(), DEC);
  SerialUSB.print(':');
  SerialUSB.println(now.second(), DEC);
  LCD.setCursor(0, 1);
  LCD.print("RTC Set!");
  delay(PAUSE);
  SerialUSB.println("Done...");
  SerialUSB.println("RTC Set Please Install Firmware");
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("RTC Set Please");
  LCD.setCursor(0, 1);
  LCD.print("Install Firmware");
}

void loop() {
  // File is for running once, not continous operation.
}
