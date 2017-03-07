##timeClock - v2.0.10-alpha
---
###An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.

*Arduino Uno, SD Shield, and RGB LCD Shield are required. There is a timeUtility.ino file in the utility folder you can run once to set the RTC and clear the devices onboard memory.*

timeClock has 6 projects that can be selected using the UP / DOWN buttons. Back-light color will change for each Project and Project Name will be displayed. Time and date will always be displayed on the Main Screen. To display the Project Selection Screen at any time, press LEFT button.
Project names can be written to the SD card in the projects.txt file. Currently they are limited to 8 characters and are loaded on startup. See example projects.txt file in the utility folder. 

To start a timer press SELECT. The back-light will change to red notifying you the timer is running. The project title, start time, and date will be logged to the SD card using a CSV file format. Pressing SELECT again will stop the timer. Project title, stop time, date, and elapsed timer will be logged to the SD card. Time in log file is in 24-hour format.
To view the Elapsed Timer Screen while timer is running, press the RIGHT button at any time. Elapsed Timer Screen will be displayed for 5 seconds and return to the Main Screen. Timer can run a maximum of 99 hours, 59 minutes, and 59 seconds. It will automatically stop at this point and log the date and time to the SD card.

To select between 12/24 time format, on Main Screen press RIGHT button.

Device records a Heartbeat to memory every 5 seconds, in case of loss of power. When a new log file is created, the heartbeat will be time-stamped at the top of the log file.

Author: Chris Frishkorn

Version Tracking
---
**March 6th, 2017     - v2.0.10-alpha  -** *Started work on issue #102.*

**March 2nd, 2017     - v2.0.9-alpha   -** *Date change now prints new date to serial interface (issue #116).*

**March 1st, 2017     - v2.0.8-alpha   -** *Optimized memory by moving dashed line strings into for loops (issue #114)*

**January 23rd, 2017  - v2.0.7-alpha   -** *Fixed Project Selection Screen error (issue #113). Moved some code into functions (issue #95).*

**January 22nd, 2017  - v2.0.6-alpha   -** *Removed file timeExample.xlsm (issue #110). Started work on issue #95, two functions added.*

**January 22nd, 2017  - v2.0.6-alpha   -** *Updated serialOutput.txt, projects.txt, and fixed README.md (issues #107, #108 & #106).* 

**December 20th, 2016 - v2.0.5-alpha   -** *Updated serial output and log file formatting (issue #96).*

**October 30th, 2016  - v2.0.4-alpha   -** *0 added to seconds in logFile heartbeat (issue #101).*

**October 28th, 2016  - v2.0.3-alpha   -** *Fixed Uninitialized projects.txt File from rendering blank projects (issue #65).*

**October 27th, 2016  - v2.0.3-alpha   -** *Removed references to hex values for ASCII, moved timerState to the correct location.*

**October 26th, 2016  - v2.0.2-alpha   -** *Fixed Carriage Return LCD rendering problem (issue #92).*

**September 5th, 2016 - v2.0.1-alpha   -** *Added seconds to heartbeat resolution (issue #90).*

**August 23rd, 2016   - v2.0.0-release -** *Released version 2.0.*

- See GitHub commit history for full version tracking notes.
