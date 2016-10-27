/*timeClock

  An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.
  Current version 2.0.2-alpha by Chris Frishkorn.

  Track this project on GitHub: https://github.com/frishkorn/timeClock

  Version Tracking
  -----------------------
  October 26th, 2016   - v2.0.2-alpha   - Fixed Carriage Return LCD rendering problem (issue #92).
  September 5th, 2016  - v2.0.1-alpha   - Added seconds to heartbeat resolution (issue #90).
  August 23rd, 2016    - v2.0.0-release - Released version 2.0.

  - See GitHub for older version tracking notes.

*/

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Adafruit_RGBLCDShield.h>

#define MAX_INTERVAL 360000
#define SYNC_INTERVAL 5000
#define TIME_OUT 1500
#define PAUSE 100

uint32_t syncTime, timerStart;
uint8_t colorSelect = 7, projectSelect = 1, timerState, prevState, timeFormat;
const uint8_t chipSelect = 10;
char projectName[7][9];

RTC_DS1307 RTC;
File logFile;
Adafruit_RGBLCDShield LCD = Adafruit_RGBLCDShield();

void dateTime(uint16_t *date, uint16_t *time) {
  // Set file date / time from RTC for SD card FAT time.
  DateTime now = RTC.now();
  *date = FAT_DATE(now.year(), now.month(), now.day());
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

void error(char *str) {
  // Display error messages to LCD and over serial interface.
  LCD.clear();
  LCD.print(F("System Error!"));
  LCD.setCursor(0, 1);
  LCD.print(str);
  Serial.print(F("Error: "));
  Serial.println(str);
  while (1);
}

void setup() {
  // Intialize Serial, I2C, LCD communications.
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  LCD.begin(16, 2);
  LCD.clear();
  LCD.setBacklight(colorSelect);
  LCD.setCursor(2, 0);
  LCD.print(F("timeClock")); // Version splash screen.
  LCD.setCursor(7, 1);
  LCD.print(F("v2.0.2a"));
  Serial.println(F("----------------------"));
  Serial.println(F("timeClock v2.0.2-alpha"));
  Serial.println(F("----------------------"));
  if (!RTC.isrunning()) {
    error("RTC Not Set");
    Serial.println(F("RTC is NOT running!"));
  }

  // See if the SD card is readable.
  Serial.print(F("SD card initializing... "));
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    error("Card Read Error");
  }
  delay(TIME_OUT); // Delay before reading files.
  Serial.println(F("Done."));

  // Read from projects.txt file and set Project Names
  File projects = SD.open("projects.txt");
  Serial.print(F("Reading projects.txt file... "));
  if (projects) {
    for (uint8_t h = 0; h < 6; h++) {
      if (projects.available()) {
        String line = projects.readStringUntil('\n');
        line.toCharArray(projectName[h], 9);
      }
    }
  }
  projects.close();
  delay(TIME_OUT); // Delay before opening another file.
  Serial.println(F("Done."));

  // LCD cannot render character 0xD (CR), replace with 0x20 (SPACE).
  for (uint8_t a = 0; a < 6; a++) {
    for (uint8_t b = 0; b < 8; b++) {
      if (projectName [a][b] == 13) {
        projectName [a][b] = 32;
      }
    }
  }

  // Create logfile.
  SdFile::dateTimeCallback(dateTime); // Set file date / time from RTC for SD card.
  char filename[] = "RECORD00.CSV";
  for (uint8_t i = 1; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (!SD.exists(filename)) {
      Serial.print(F("Creating file "));
      Serial.print(filename);
      Serial.print(F("... "));
      logFile = SD.open(filename, FILE_WRITE);
      delay(TIME_OUT); // Delay before writing another file.
      Serial.println(F("Done."));
      break;
    }
  }
  if (!logFile) {
    error("File Write Error");
  }

  // Print Date over Serial Interface
  DateTime now = RTC.now(); // Get current time and date from RTC.
  Serial.println(F("----------------"));
  Serial.print(F("Date: "));
  if (now.month() < 10) { // If month is a single digit precede with a zero.
    Serial.print("0");
  }
  Serial.print(now.month(), DEC);
  Serial.print('/');
  if (now.day() < 10) { // If day is a single digit precede with a zero.
    Serial.print("0");
  }
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.println(now.year(), DEC);
  Serial.println(F("----------------"));

  // Read last heartbeat from NV_SRAM and write header to top of log-file.
  logFile.print(F("Last heartbeat detected: "));
  if (RTC.readnvram(2) != 255 && RTC.readnvram(7) != 255) {
    logFile.print(RTC.readnvram(2), DEC); // Print Month to log-file.
    logFile.print("/");
    logFile.print(RTC.readnvram(3), DEC); // Print Day to log-file.
    logFile.print("/");
    logFile.print(RTC.readnvram(4), DEC); // Print Year to log-file.
    logFile.print(RTC.readnvram(5), DEC);
    logFile.print(F(" @ "));
    if (RTC.readnvram(6) < 10) {
      logFile.print("0");
    }
    logFile.print(RTC.readnvram(6), DEC); // Print Hour to log-file.
    logFile.print(":");
    if (RTC.readnvram(7) < 10) {
      logFile.print("0");
    }
    logFile.print(RTC.readnvram(7), DEC); // Print Minute to log-file.
    logFile.print(":");
    logFile.println(RTC.readnvram(8), DEC); // Print Second to log-file.
  }
  else {
    logFile.println(F("None"));
  }
  logFile.println(F("Date,Time,Project"));
  delay(TIME_OUT);
  LCD.clear();

  // Read first byte of NV_SRAM set colorSelect and projectSelect.
  colorSelect = RTC.readnvram(0);
  projectSelect = RTC.readnvram(1);
  if (colorSelect == 255 && projectSelect == 255) { // Set to defaults if RTC has been recently set and NV_SRAM wiped.
    colorSelect = 7;
    projectSelect = 1;
  }
  LCD.setBacklight(colorSelect);

  // Read previously set timeFormat.
  timeFormat = RTC.readnvram(9);
  if (timeFormat > 1) {
    timeFormat = 0;
  }
}

void loop() {
  // Use UP/DOWN buttons to change RGB backlight color and select project.
  uint8_t buttons = LCD.readButtons();
  if (timerState == 0) { // Prevent user from changing project while timer is active.
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
        LCD.setCursor(2, 0);
        uint8_t i = projectSelect - 1;
        LCD.print(projectName[i]);
        LCD.setCursor(6, 1);
        LCD.print(F("Selected"));
        delay(TIME_OUT);
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
        LCD.setCursor(2, 0);
        uint8_t i = projectSelect - 1;
        LCD.print(projectName[i]);
        LCD.setCursor(6, 1);
        LCD.print(F("Selected"));
        delay(TIME_OUT);
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
    uint8_t i = projectSelect - 1;
    logFile.println(projectName[i]);
    LCD.clear();
    LCD.setCursor(1, 0);
    LCD.print(F("Data logged to"));
    LCD.setCursor(2, 1);
    LCD.print(F("media device"));

    // Timer starts with the first press of the SELECT BUTTON.
    timerState = 1 - timerState;
    if (timerState == 1 && prevState == 0) {
      timerStart = now.secondstime(); // Time from RTC in seconds since 1/1/2000.
      delay(TIME_OUT);
      LCD.setBacklight(1);
      LCD.clear();
      LCD.setCursor(3, 0);
      LCD.print(F("Timer"));
      LCD.setCursor(5, 1);
      LCD.print(F("Started!"));
      Serial.print(F("Timer Started: "));
      if (now.hour() < 10) {
        Serial.print("0");
      }
      Serial.print(now.hour(), DEC);
      Serial.print(":");
      if (now.minute() < 10) {
        Serial.print("0");
      }
      Serial.print(now.minute(), DEC);
      Serial.print(":");
      if (now.second() < 10) {
        Serial.print("0");
      }
      Serial.println(now.second(), DEC);
    }

    // Timer stops with the second press of the SELECT BUTTON.
    if (timerState == 0 && prevState == 1) {
      uint32_t timerStop = now.secondstime();
      uint32_t timerTime = timerStop - timerStart;
      uint8_t ss = timerTime % 60;
      uint8_t mm = (timerTime / 60) % 60;
      uint8_t hh = (timerTime / 3600);
      logFile.print(F("Timer"));
      logFile.print(",");
      logFile.print(F("hh:mm:ss"));
      logFile.print(",");
      if (hh < 10) {
        logFile.print("0");
      }
      logFile.print(hh, DEC);
      logFile.print(":");
      if (mm < 10) {
        logFile.print("0");
      }
      logFile.print(mm, DEC);
      logFile.print(":");
      if (ss < 10) {
        logFile.print("0");
      }
      logFile.println(ss, DEC);
      delay(TIME_OUT);
      LCD.setBacklight(colorSelect);
      LCD.clear();
      LCD.setCursor(3, 0);
      LCD.print(F("Timer"));
      LCD.setCursor(5, 1);
      LCD.print(F("Stopped!"));
      if (hh < 10) {
        Serial.print("0");
      }
      Serial.print(hh, DEC);
      Serial.print(":");
      if (mm < 10) {
        Serial.print("0");
      }
      Serial.print(mm, DEC);
      Serial.print(":");
      if (ss < 10) {
        Serial.print("0");
      }
      Serial.print(ss, DEC);
      Serial.print(F(" - "));
      Serial.println(projectName[i]);
      Serial.println("---");
    }
    prevState = timerState;
    delay(TIME_OUT);
  }

  // Show Project Name on LCD when user presses LEFT BUTTON.
  if (buttons & BUTTON_LEFT) {
    LCD.clear();
    LCD.setCursor(2, 0);
    uint8_t i = projectSelect - 1;
    LCD.print(projectName[i]);
    LCD.setCursor(6, 1);
    LCD.print(F("Selected"));
    delay(TIME_OUT);
  }

  // Show Elapsed Timer time on LCD when user presses RIGHT BUTTON, only while timer is running.
  if (timerState == 1) {
    if (buttons & BUTTON_RIGHT) {
      LCD.clear();
      for (uint8_t i = 0; i < 25; i++) { // Refresh display fast enough to show counting seconds for ~5 seconds.
        DateTime now = RTC.now(); // Get current time and date from RTC.
        LCD.setCursor(0, 0);
        LCD.print(F("Elapsed "));
        uint32_t timerStop = now.secondstime();
        uint32_t timerTime = timerStop - timerStart;
        uint8_t ss = timerTime % 60;
        uint8_t mm = (timerTime / 60) % 60;
        uint8_t hh = (timerTime / 3600);
        if (hh < 10) {
          LCD.print("0");
        }
        LCD.print(hh);
        LCD.print(":");
        if (mm < 10) {
          LCD.print("0");
        }
        LCD.print(mm);
        LCD.print(":");
        if (ss < 10) {
          LCD.print("0");
        }
        LCD.print(ss);
        LCD.setCursor(1, 1);
        LCD.print(F("Timer  hh:mm:ss"));
        delay(5);
      }
      LCD.clear();
    }
  }

  // If Elapsed Timer reaches 99:59:59 stop timer.
  if (timerState == 1) {
    DateTime now = RTC.now();
    uint32_t maxTimer = now.secondstime();
    if ((maxTimer - timerStart) >= MAX_INTERVAL) {
      uint32_t timerStop = now.secondstime();
      uint32_t timerTime = timerStop - timerStart;
      uint8_t ss = timerTime % 60;
      uint8_t mm = (timerTime / 60) % 60;
      uint8_t hh = (timerTime / 3600);
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
      uint8_t i = projectSelect - 1;
      logFile.println(projectName[i]);
      logFile.print(F("Timer"));
      logFile.print(",");
      logFile.print(F("hh:mm:ss"));
      logFile.print(",");
      if (hh < 10) {
        logFile.print("0");
      }
      logFile.print(hh, DEC);
      logFile.print(":");
      if (mm < 10) {
        logFile.print("0");
      }
      logFile.print(mm, DEC);
      logFile.print(":");
      if (ss < 10) {
        logFile.print("0");
      }
      logFile.println(ss, DEC);
      LCD.setBacklight(colorSelect);
      LCD.clear();
      LCD.setCursor(2, 0);
      LCD.print(F("Timer Maxed!"));
      LCD.setCursor(4, 1);
      LCD.print(projectName[i]);
      delay(TIME_OUT);
      timerState = 0;
      prevState = 0;
    }
  }

  // Allow user to change time format between 12/24 hours while timer is not active with RIGHT button.
  if (timerState == 0) {
    if (buttons & BUTTON_RIGHT) {
      timeFormat = 1 - timeFormat;
      delay(PAUSE);
    }
  }

  // Display Date and Time on LCD in 24 hour format.
  if (timeFormat == 1) {
    DateTime now = RTC.now(); // Get current time and date from RTC.
    LCD.setCursor(0, 0);
    LCD.print(F("Date "));
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
    LCD.print(F("Time "));
    if (now.hour() < 10) {
      LCD.print("0");
    }
    LCD.print(now.hour(), DEC);
    LCD.print(":");
    if (now.minute() < 10) {
      LCD.print("0");
    }
    LCD.print(now.minute(), DEC);
    LCD.print(":");
    if (now.second() < 10) {
      LCD.print("0");
    }
    LCD.print(now.second(), DEC);
    LCD.print(" HR");
  }
  else {
    // Display Date and Time on LCD in 12 hour format.
    DateTime now = RTC.now(); // Get current time and date from RTC.
    LCD.setCursor(0, 0);
    LCD.print(F("Date "));
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
    LCD.print(F("Time "));
    if (now.hour() > 12) { // RTC is in 24 hour format, subtract 12 for 12 hour time.
      if (now.hour() < 22) { // Don't precede with a zero for 10:00 & 11:00 PM.
        LCD.print("0");
      }
      LCD.print(now.hour() - 12, DEC);
    }
    else {
      if (now.hour() == 0) { // Set 00:00 AM to 12:00 AM.
        LCD.print(now.hour() + 12, DEC);
      }
      else {
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
      LCD.print(F(" PM"));
    }
    else {
      LCD.print(F(" AM"));
    }
  }

  // Write data to card, only if 5 seconds has elasped since last write.
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  logFile.flush();

  // Write heartbeat to NV_SRAM.
  DateTime now = RTC.now(); // Get current time and date from RTC.
  RTC.writenvram(2, now.month());
  RTC.writenvram(3, now.day());
  uint16_t fyear = now.year();
  uint8_t hiyear = fyear / 100;
  uint8_t loyear = fyear - 2000;
  RTC.writenvram(4, hiyear);
  RTC.writenvram(5, loyear);
  RTC.writenvram(6, now.hour());
  RTC.writenvram(7, now.minute());
  RTC.writenvram(8, now.second());
  RTC.writenvram(9, timeFormat);
}
