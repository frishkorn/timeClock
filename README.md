##timeClock - v1.6.4-beta
---
###An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.

*Arduino Uno, SD Shield, and RGB LCD Shield are required. There is a utility in the utility folder you can run once to set the RTC.*

timeClock has 6 projects that can be selected using the UP / DOWN buttons. Back-light color will change for each Project and Project Name will be displayed. Time and date will always be displayed on the Main Screen. To display the Project Name at any time, press LEFT button.
Project names can be written to the SD card in the projects.txt file. Currently they are limited to 8 characters. See example file in utility folder. 

To start a timer press SELECT, the back-light will change red notifying you the timer is running. The project title, start time, and date will be logged to the SD card using a CSV file. Pressing SELECT again will stop the timer, the project title and stop time will be logged to the SD card in addition to the elapsed timer. Time in log file is in 24-hour format.
To view the Elapsed Timer while timer is running, press the RIGHT button at any time. Elapsed timer can run a maximum of 99 hours, 59 minutes, and 59 seconds.

To select between 12/24 time formats, while timer is not running press RIGHT button.

Device records a heartbeat every 5 seconds in case of loss of power. When a new log file is created, the heartbeat will be time-stamped at the top of the log file.

Author: Chris Frishkorn

Version Tracking
---
**July 31st, 2016     - v1.6.4-beta    -** *Project names are now read line by line from projects.txt (issue #74).*

**July 30th, 2016     - v1.6.3-alpha   -** *Updated serial output (issue #87).*

**May 15th, 2016      - v1.6.2-alpha   -** *Fixed missing zero from 24 hour time (issue #83).*

**May 14th, 2016      - v1.6.1-alpha   -** *Fixed timeFormat initialation issue with RTC reset (issue #81).*

**May 14th, 2016      - v1.6.0-alpha   -** *Added select 12/24 time format option (issue #7) and reduced TIME_OUT.*

**April 21st, 2016    - v1.5.6-alpha   -** *Fixed Elapsed Timer roll-over problem, minor UI update (issue #78).*

**March 24th, 2016    - v1.5.5-alpha   -** *Strings moved to PROGMEM, SRAM memory savings. 78% to 61% SRAM (issue #76).*

**March 24th, 2016    - v1.5.4-alpha   -** *Minor UI update.*

**March 12th, 2016    - v1.5.3-alpha   -** *Removed heartbeat from Serial output, moved serial output after LCD output (issue #72).*

**March 7th, 2016     - v1.5.2-alpha   -** *Last heartbeat added to Serial output (issue #70).*

**March 7th, 2016     - v1.5.1-alpha   -** *Minor UI adjustments, updated Serial output (issue #63).*

**March 6th, 2016     - v1.5.0-alpha   -** *Added RIGHT button press show Elapsed Timer (issue #62).*

**March 6th, 2016     - v1.4.0-alpha   -** *Added Project Names which are loaded from the SD card (issue #40).*

**February 11th, 2016 - v1.3.1-alpha   -** *Changed time-out constant to use pre-processor #define (issue #60).*

**February 10th, 2016 - v1.3.0-alpha   -** *Added LEFT button press Project Name notification, updated variables (issue #33).*

**February 9th, 2016  - v1.2.4-alpha   -** *Fixed RTC Error message and optimized heartbeat log-file write code (issue #55).*

**February 7th, 2016  - v1.2.3-alpha   -** *Fixed uninitialized heartbeat and updated sync interval (issue #51 & issue #54).*

**February 7th, 2016  - v1.2.2-alpha   -** *Fixed heartbeat, now has zeros appened to log file (issue #49).*

**February 7th, 2016  - v1.2.1-alpha   -** *Fixed RTC reset problem with colorSelect and projectSelect (issue #48).*

**February 6th, 2016  - v1.2.0-release -** *Released version 1.2.*

- See GitHub commit history for full version tracking notes.