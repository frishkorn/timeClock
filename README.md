timeClock - v2.2.3-alpha
---
**An Arduino Zero driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.**

*Arduino Zero, SD Shield (Rev B), and RGB LCD Shield are required. There is a timeUtility.ino file in the utility folder you can run once to set the RTC.*

timeClock has 6 projects that can be selected using the UP / DOWN buttons. Back-light color will change for each Project and Project Name will be displayed. Time and date will always be displayed on the Main Screen. To display the Project Selection Screen at any time, press LEFT button.
Project names can be written to the SD card in the projects.txt file. Currently they are limited to 8 characters and are loaded on startup. See example projects.txt file in the utility folder. 

To start a timer press SELECT. The back-light will change to red notifying you the timer is running. The project title, start time, and date will be logged to the SD card using a CSV file format. Pressing SELECT again will stop the timer. Project title, stop time, date, and elapsed timer will be logged to the SD card. Time in log file is in 24-hour format.
To view the Elapsed Timer Screen while timer is running, press the RIGHT button at any time. Elapsed Timer Screen will be displayed for 5 seconds and return to the Main Screen. Timer can run a maximum of 99 hours, 59 minutes, and 59 seconds. It will automatically stop at this point and log the date and time to the SD card.

While the timer is active, a 15 minute interval will flash. The screen will blink 3 times to capture the attention of the user. Then the screen will blink the number of times the 15 minute interval has passed. Once timer has stopped the interval flash will reset. This is to give the user an idea of how much time has passed without reaching up to press the RIGHT button.

To select between 12/24 time format, on Main Screen press RIGHT button.

Author: Chris Frishkorn

Version Tracking
---
**November 23rd, 2019  - v2.2.3-alpha   -** *Change serial output port (issue #139).*

**April 16th, 2017     - v2.2.2-alpha   -** *Code has been ported to use Arduino Zero board (issue #127).*

**April 15th, 2017     - v2.2.2-alpha   -** *Changed NVRAM operations to Flash EEPROM (issue #135).*

**April 14th, 2017     - v2.2.1-alpha   -** *Fixed serial and reset hang (issue #136).*

**April 13th, 2017     - v2.2.0-alpha   -** *New SD card shield is now working with code (issue #134).*

**March 15th, 2017     - v2.1.1-release -** *Released version 2.1.1.*

- See GitHub commit history for full version tracking notes.
