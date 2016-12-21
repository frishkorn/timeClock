/*timeUtility

  Utility to set DS1307 RTC to System Time and clear NV_SRAM.
  Current version 0.0.2-alpha by Chris Frishkorn.

  Version Release History
  -----------------------
  May 15th, 2016      - v0.0.2-alpha - Added LCD output (issue #84).
  December 28th, 2015 - v0.0.1-alpha - Utility created.
*/

#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include <Adafruit_RGBLCDShield.h>

#define PAUSE 2500

RTC_DS1307 RTC;
Adafruit_RGBLCDShield LCD = Adafruit_RGBLCDShield();

// Print NV_SRAM to terminal, formatted.
void printnvram(uint8_t address) {
  Serial.print("Address 0x");
  Serial.print(address, HEX);
  Serial.print(" = 0x");
  Serial.println(RTC.readnvram(address), HEX);
}

// Error sub-routine.
void error(char *str) {
  Serial.print("Error: ");
  Serial.println(str);
  while (1);
}

void setup() {
  // Intialize Serial RTC, and I2C.
  Serial.begin(9600);
  Wire.begin();
  LCD.begin(16, 2);
  LCD.clear();
  LCD.setBacklight(7);
  LCD.setCursor(0, 0);
  LCD.print("Initializing RTC");
  RTC.begin();
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  // Set RTC time and date.
  RTC.adjust(DateTime(__DATE__, __TIME__));
  DateTime now = RTC.now();
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

  // Dump old NV_SRAM contents on startup.
  Serial.println("Current NVRAM values:");
  LCD.setCursor(0, 1);
  LCD.print("Dumping NVRAM!");
  for (uint8_t i = 0; i < 55; ++i) {
    printnvram(i);
    delay(250);
  }
  delay(PAUSE);
  Serial.println("Done...");

  // Set all memory addresses to OxFF.
  delay(PAUSE);
  Serial.println("Clearing NV_SRAM");
  LCD.setCursor(0, 1);
  LCD.print("Clearing NVRAM!");
  for (uint8_t i = 0; i < 55; ++i) {
    RTC.writenvram(i, 0xFF);
  }
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
