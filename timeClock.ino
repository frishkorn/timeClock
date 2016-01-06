/*timeClock

  An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.
  Current version 0.4.1-alpha by Chris Frishkorn.

  Version Release History
  -----------------------
  January 5th, 2016   - v0.4.1-alpha - Started work on issue #3.
  January 3rd, 2016   - v0.4.0-alpha - Added project memory, when device resets last project will be loaded (issue #6).
  January 3rd, 2016   - v0.3.3-alpha - Issue fixed to prevent user from leaving project once timer begins (issue #18).
  December 29th, 2015 - v0.3.2-alpha - Fixed error messages, LCD now displays errors (issue #17).
  December 28th, 2015 - v0.3.1-alpha - Optimized code around timers, removed RTC date time set functions (issue #10).
  December 27th, 2015 - v0.3.0-alpha - Added select Project ability with notification (issue #4).
  December 27th, 2015 - v0.2.1-alpha - Added Start / Stop LCD notification (issue #13).
  December 23rd, 2015 - v0.2.0-alpha - Start / Stop timer added to SELECT button (issue #2).
  December 22nd, 2015 - v0.1.4-alpha - Fixed when time is equal to 10 AM or 11 AM a 0 is preceded before the hours digit (issue #8).
  December 21st, 2015 - v0.1.3-alpha - Removed useless debouncing and delay, discovered library handles it internally.
  December 21st, 2015 - v0.1.2-alpha - Updated data-types across code and removed ECHO_TO_SERIAL debugging (#issue 5).
  December 21st, 2015 - v0.1.1-alpha - Added code to dump NVRAM from DS1307 on startup.
  December 21st, 2015 - v0.0.1-alpha - Code forked from arduinoTSens which runs the underlying RTC, LCD, and SD Arduino shields.
*/

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <Adafruit_RGBLCDShield.h>

RTC_DS1307 RTC;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define LOG_INTERVAL 5000
#define SYNC_INTERVAL 5000

uint32_t syncTime = 0;
uint32_t timerStart = 0;
uint32_t timerStop = 0;
uint8_t timerState = 0;
uint8_t prevState = 0;
uint8_t colorSelect = 7;
uint8_t projectSelect = 1;
const int chipSelect = 10;

File logFile;

void error(char *str) {
  lcd.clear();
  lcd.print("System Error!");
  lcd.setCursor(0, 1);
  lcd.print(str);
  Serial.print("Error: ");
  Serial.println(str);
  while (1);
}

void setup() {
  // Intialize Serial, I2C, LCD Communication.
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);
  lcd.setBacklight(colorSelect);
  lcd.print("timeClock");
  lcd.setCursor(0, 1);
  lcd.print("    v0.4.1-alpha");
  RTC.begin();
  if (!RTC.isrunning()) {
    error("RTC Stopped");
    Serial.println("RTC is NOT running!");
  }

  // Check and see if the SD card is readable.
  Serial.println();
  Serial.print("SD card initializing... ");
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    error("Card Read Error");
  }
  Serial.println("Card sucessfully initialized.");

  // Create logfile.
  char filename[] = "RECORD00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (! SD.exists(filename)) {
      logFile = SD.open(filename, FILE_WRITE);
      break;
    }
  }
  if (! logFile) {
    error("File Write Error");
  }
  Serial.print("Creating file ");
  Serial.println(filename);
  Serial.println();
  logFile.println("Date,Time,Project");
  delay(3000);
  lcd.clear();

  // Read first byte of NV_SRAM set colorSelect and projectSelect.
  colorSelect = RTC.readnvram(0);
  projectSelect = RTC.readnvram(1);
  lcd.setBacklight(colorSelect);
}

void loop() {
  // Use UP/DOWN buttons to change RGB backlight color and select project.
  uint8_t buttons = lcd.readButtons();
  if (timerState == 0) { // Prevent user from pressing BUTTON_DOWN while timer is active.
    if (buttons) {
      if (buttons & BUTTON_UP) {
        if (colorSelect >= 7) {
          colorSelect = 7;
          projectSelect = 1;
          RTC.writenvram(0, colorSelect);
          RTC.writenvram(1, projectSelect);
        }
        else {
          colorSelect++;
          projectSelect--;
          RTC.writenvram(0, colorSelect);
          RTC.writenvram(1, projectSelect);
        }
        lcd.setBacklight(colorSelect);
      }
      if (buttons & BUTTON_DOWN) {
        if (colorSelect <= 2) {
          colorSelect = 2;
          projectSelect = 6;
          RTC.writenvram(0, colorSelect);
          RTC.writenvram(1, projectSelect);
        }
        else {
          colorSelect--;
          projectSelect++;
          RTC.writenvram(0, colorSelect);
          RTC.writenvram(1, projectSelect);
        }
        lcd.setBacklight(colorSelect);
      }
    }
  }

  // Log date and time information if the SELECT button is pressed.
  if (buttons & BUTTON_SELECT) {
    DateTime now = RTC.now(); // Get current time and date from RTC.
    logFile.print(now.month(), DEC);
    logFile.print('/');
    logFile.print(now.day(), DEC);
    logFile.print('/');
    logFile.print(now.year(), DEC);
    logFile.print(", ");
    logFile.print(now.hour(), DEC);
    logFile.print(':');
    logFile.print(now.minute(), DEC);
    logFile.print(':');
    logFile.print(now.second(), DEC);
    logFile.print(", ");
    logFile.print("Project ");
    logFile.println(projectSelect);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Data logged to");
    lcd.setCursor(0, 1);
    lcd.print("  media device");

    // Timer starts with the first press of the SELECT BUTTON.
    timerState = 1 - timerState;
    if (timerState == 1 && prevState == 0) {
      timerStart = millis();
      delay(1000);
      lcd.setBacklight(1);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Timer Started!");
      lcd.setCursor(0, 1);
      lcd.print("   Project 0");
      lcd.print(projectSelect);
    }
    // Timer stops with the second press of the SELECT BUTTON.
    if (timerState == 0 && prevState == 1) {
      timerStop = millis();
      logFile.print("Timer (ms)");
      logFile.print(", ");
      logFile.println(timerStop - timerStart);
      delay(1000);
      lcd.setBacklight(colorSelect);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Timer Stopped!");
      lcd.setCursor(0, 1);
      lcd.print("   Project 0");
      lcd.print(projectSelect);
    }
    prevState = timerState;
    delay(2500);
  }

  // Display Date and Time on LCD.
  DateTime now = RTC.now(); // Get current time and date from RTC.
  lcd.setCursor(0, 0);
  lcd.print("Date ");
  if (now.month() < 10) { // If month is a single digit precede with a zero.
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  lcd.print('/');
  if (now.day() < 10) { // If day is a single digit precede with a zero.
    lcd.print("0");
  }
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Time ");
  if (now.hour() > 12) { // RTC is in 24 hour format, subtract 12 for 12 hour time.
    if (now.hour() < 22) { // Don't precede with a zero for 10:00 & 11:00 PM.
      lcd.print("0");
    }
    lcd.print(now.hour() - 12, DEC);
  }
  else
  {
    if (now.hour() == 0) { // Set 00:00 AM to 12:00 AM.
      lcd.print(now.hour() + 12, DEC);
    }
    else
    {
      if (now.hour() >= 10 && now.hour() <= 12) { // Don't precede with a zero if it's between 10 AM - 12 PM.
        lcd.print(now.hour(), DEC);
      }
      else {
        lcd.print("0");
        lcd.print(now.hour(), DEC);
      }
    }
  }
  lcd.print(':');
  if (now.minute() < 10) { // If minute is a single digit precede with a zero.
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  if (now.second() < 10) { // If second is a single digit precede with a zero.
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);
  if (now.hour() > 11) {
    lcd.print(" PM");
  }
  else {
    lcd.print(" AM");
  }

  // Write data to card.
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  logFile.flush();
}

