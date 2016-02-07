/*timeClock

  An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.
  Current version 1.2.3-alpha by Chris Frishkorn.

  Track this project on GitHub: https://github.com/frishkorn/timeClock

  Version Release History
  -----------------------
  February 7th, 2016  - v1.2.3-alpha   - Fixed uninitialized heartbeat and updated sync interval (issue #51 & issue #54).
  February 7th, 2016  - v1.2.2-alpha   - Fixed heartbeat, now has zeros appened to log file (issue #49).
  February 7th, 2016  - v1.2.1-alpha   - Fixed RTC reset problem with colorSelect and projectSelect (issue #48).
  February 6th, 2016  - v1.2.0-release - Released version 1.2.
  February 3rd, 2016  - v1.1.4-beta    - Added heartbeat to log file. (issue #36 & issue #44).
  January 23rd, 2016  - v1.1.3-alpha   - Project select boundary condition fixed, timers made consistant across code (issue #32).
  January 22nd, 2016  - v1.1.2-alpha   - Fixed timer accuracy, improved log format (issue #38 & issue #39).
  January 10th, 2016  - v1.1.1-alpha   - Improved log format, changed timer to hh:mm:ss format (issue #34 & issue #27).
  January 10th, 2016  - v1.1.0-alpha   - Added project notfication when pressing UP/DOWN buttons (issue #30).
  January 7th, 2016   - v1.0.0-release - Released version 1.0.
  - See GitHub for older version release notes.
*/

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <Adafruit_RGBLCDShield.h>

#define SYNC_INTERVAL 10000

uint32_t syncTime, timerStart, timerStop, timerTime, timerState, prevState;
uint8_t colorSelect = 7, projectSelect = 1;
const uint8_t chipSelect = 10;

RTC_DS1307 RTC;
File logFile;
Adafruit_RGBLCDShield LCD = Adafruit_RGBLCDShield();

void dateTime(uint16_t *date, uint16_t *time) {
  // Set file date / time from RTC for SD card.
  DateTime now = RTC.now();
  *date = FAT_DATE(now.year(), now.month(), now.day());
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

void error(char *str) {
  // Display error messages to LCD and over serial interface.
  LCD.clear();
  LCD.print("System Error!");
  LCD.setCursor(0, 1);
  LCD.print(str);
  Serial.print("Error: ");
  Serial.println(str);
  while (1);
}

void setup() {
  // Intialize serial, I2C, LCD communication.
  Serial.begin(9600);
  Wire.begin();
  LCD.begin(16, 2);
  LCD.setBacklight(colorSelect);
  LCD.setCursor(2, 0);
  LCD.print("timeClock");
  LCD.setCursor(7, 1);
  LCD.print("v1.2.3a");
  RTC.begin();
  if (!RTC.isrunning()) {
    error("RTC Stopped");
    Serial.println("RTC is NOT running!");
  }

  // See if the SD card is readable.
  Serial.println();
  Serial.print("SD card initializing... ");
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    error("Card Read Error");
  }
  Serial.println("Card sucessfully initialized.");

  // Create logfile.
  SdFile::dateTimeCallback(dateTime); // Set file date / time from RTC for SD card.
  char filename[] = "RECORD00.CSV";
  for (uint8_t i = 1; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (!SD.exists(filename)) {
      logFile = SD.open(filename, FILE_WRITE);
      break;
    }
  }
  if (!logFile) {
    error("File Write Error");
  }
  Serial.print("Creating file ");
  Serial.println(filename);
  Serial.println();

  // Read last heartbeat from NV_SRAM and write to top of logfile.
  uint8_t rammm = RTC.readnvram(2);
  uint8_t ramdd = RTC.readnvram(3);
  uint8_t ramyy = RTC.readnvram(4);
  uint8_t ramrr = RTC.readnvram(5);
  uint8_t ramhr = RTC.readnvram(6);
  uint8_t rammi = RTC.readnvram(7);
  logFile.print("Last heartbeat detected: ");
  if (rammm != 255 && rammi != 255) {
    logFile.print(rammm);
    logFile.print("/");
    logFile.print(ramdd);
    logFile.print("/");
    logFile.print(ramyy);
    logFile.print(ramrr);
    logFile.print(" @ ");
    if (ramhr < 10) {
      logFile.print("0");
    }
    logFile.print(ramhr);
    logFile.print(":");
    if (rammi < 10) {
      logFile.print("0");
    }
    logFile.println(rammi);
  } else {
    logFile.println("None");
  }
  logFile.println("Date,Time,Project");
  delay(3000);
  LCD.clear();

  // Read first byte of NV_SRAM set colorSelect and projectSelect.
  colorSelect = RTC.readnvram(0);
  projectSelect = RTC.readnvram(1);
  if (colorSelect == 255 && projectSelect == 255) { // Set to defaults if RTC has been recently set and NV_SRAM wiped.
    colorSelect = 7;
    projectSelect = 1;
  }
  LCD.setBacklight(colorSelect);
}

void loop() {
  // Use UP/DOWN buttons to change RGB backlight color and select project.
  uint8_t buttons = LCD.readButtons();
  if (timerState == 0) { // Prevent user from changing project while timer is active.
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
          LCD.setBacklight(colorSelect);
          LCD.clear();
          LCD.setCursor(3, 0);
          LCD.print("Project 0");
          LCD.print(projectSelect);
          LCD.setCursor(4, 1);
          LCD.print("Selected");
          delay(1500);
        }
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
          LCD.setBacklight(colorSelect);
          LCD.clear();
          LCD.setCursor(3, 0);
          LCD.print("Project 0");
          LCD.print(projectSelect);
          LCD.setCursor(4, 1);
          LCD.print("Selected");
          delay(1500);
        }
      }
    }
  }

  // Log date and time information if the SELECT button is pressed.
  if (buttons & BUTTON_SELECT) {
    DateTime now = RTC.now(); // Get current time and date from RTC.
    if (now.month() < 10) {
      logFile.print("0");
    }
    logFile.print(now.month(), DEC);
    logFile.print('/');
    if (now.day() < 10) {
      logFile.print("0");
    }
    logFile.print(now.day(), DEC);
    logFile.print('/');
    logFile.print(now.year(), DEC);
    logFile.print(",");
    if (now.hour() < 10) {
      logFile.print("0");
    }
    logFile.print(now.hour(), DEC);
    logFile.print(':');
    if (now.minute() < 10) {
      logFile.print("0");
    }
    logFile.print(now.minute(), DEC);
    logFile.print(':');
    if (now.second() < 10) {
      logFile.print("0");
    }
    logFile.print(now.second(), DEC);
    logFile.print(",");
    logFile.print("Project ");
    logFile.println(projectSelect);
    LCD.clear();
    LCD.setCursor(1, 0);
    LCD.print("Data logged to");
    LCD.setCursor(2, 1);
    LCD.print("media device");

    // Timer starts with the first press of the SELECT BUTTON.
    timerState = 1 - timerState;
    if (timerState == 1 && prevState == 0) {
      timerStart = now.secondstime(); // Time from RTC in seconds since 1/1/2000.
      delay(1500);
      LCD.setBacklight(1);
      LCD.clear();
      LCD.setCursor(1, 0);
      LCD.print("Timer Started!");
      LCD.setCursor(3, 1);
      LCD.print("Project 0");
      LCD.print(projectSelect);
    }

    // Timer stops with the second press of the SELECT BUTTON.
    if (timerState == 0 && prevState == 1) {
      timerStop = now.secondstime();
      timerTime = timerStop - timerStart;
      uint8_t ss = timerTime % 60;
      uint8_t mm = (timerTime / 60) % 60;
      uint8_t hh = (timerTime / 3600);
      logFile.print("Timer");
      logFile.print(",");
      logFile.print("hh:mm:ss");
      logFile.print(",");
      if (hh < 10) {
        logFile.print("0");
      }
      logFile.print(hh);
      logFile.print(":");
      if (mm < 10) {
        logFile.print("0");
      }
      logFile.print(mm);
      logFile.print(":");
      if (ss < 10) {
        logFile.print("0");
      }
      logFile.println(ss);
      delay(1500);
      LCD.setBacklight(colorSelect);
      LCD.clear();
      LCD.setCursor(1, 0);
      LCD.print("Timer Stopped!");
      LCD.setCursor(3, 1);
      LCD.print("Project 0");
      LCD.print(projectSelect);
    }
    prevState = timerState;
    delay(1500);
  }

  // Display Date and Time on LCD.
  DateTime now = RTC.now(); // Get current time and date from RTC.
  LCD.setCursor(0, 0);
  LCD.print("Date ");
  if (now.month() < 10) { // If month is a single digit precede with a zero.
    LCD.print("0");
  }
  LCD.print(now.month(), DEC);
  LCD.print('/');
  if (now.day() < 10) { // If day is a single digit precede with a zero.
    LCD.print("0");
  }
  LCD.print(now.day(), DEC);
  LCD.print('/');
  LCD.print(now.year(), DEC);
  LCD.setCursor(0, 1);
  LCD.print("Time ");
  if (now.hour() > 12) { // RTC is in 24 hour format, subtract 12 for 12 hour time.
    if (now.hour() < 22) { // Don't precede with a zero for 10:00 & 11:00 PM.
      LCD.print("0");
    }
    LCD.print(now.hour() - 12, DEC);
  }
  else
  {
    if (now.hour() == 0) { // Set 00:00 AM to 12:00 AM.
      LCD.print(now.hour() + 12, DEC);
    }
    else
    {
      if (now.hour() >= 10 && now.hour() <= 12) { // Don't precede with a zero if it's between 10 AM - 12 PM.
        LCD.print(now.hour(), DEC);
      }
      else {
        LCD.print("0");
        LCD.print(now.hour(), DEC);
      }
    }
  }
  LCD.print(':');
  if (now.minute() < 10) { // If minute is a single digit precede with a zero.
    LCD.print("0");
  }
  LCD.print(now.minute(), DEC);
  LCD.print(':');
  if (now.second() < 10) { // If second is a single digit precede with a zero.
    LCD.print("0");
  }
  LCD.print(now.second(), DEC);
  if (now.hour() > 11) {
    LCD.print(" PM");
  }
  else {
    LCD.print(" AM");
  }

  // Write data to card.
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  logFile.flush();

  // Write heartbeat to NV_SRAM.
  RTC.writenvram(2, now.month());
  RTC.writenvram(3, now.day());
  uint16_t fyear = now.year();
  uint8_t hiyear = fyear / 100;
  uint8_t loyear = fyear - 2000;
  RTC.writenvram(4, hiyear);
  RTC.writenvram(5, loyear);
  RTC.writenvram(6, now.hour());
  RTC.writenvram(7, now.minute());
}
