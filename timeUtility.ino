/*timeUtility

  Utility to set DS1307 RTC to System Time and clear NV_SRAM.
  Current version 0.0.1-alpha by Chris Frishkorn.

  Version Release History
  -----------------------
  December 28th, 2015 - v0.0.1-alpha - Started work on issue #10.
*/

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

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
  delay(1500);

  // Dump old NV_SRAM contents on startup.
  Serial.println("Current NVRAM values:");
  delay(1500);
  for (uint8_t i = 0; i < 55; ++i) {
    printnvram(i);
    delay(250);
  }
  delay(1500);
  Serial.println("Done...");
}

void loop() {
  // Set all memory addresses to OxFF
}
