##timeClock - v1.2.3-alpha
---
###An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.

*Arduino Uno, SD Shield, and RGB LCD Shield are required. There is a utility in the utility folder you can run once to set the RTC.*

timeClock has 6 projects that can be selected using the UP / DOWN buttons. Back-light color will change for each Project and Project Number will be displayed. Time and date will always be displaying on the Main Screen.

To start a timer press SELECT, the back-light will change red notifying you the timer is running. The project title, start time, and date will be logged to the SD card using a CSV file. Pressing SELECT again will stop the timer, the project title and stop time will be logged to the SD card in addition to the elapsed timer. Time in log file is in 24-hour format.

Device records a heartbeat every 15 seconds in case of loss of power. When a new log file is created, the heartbeat will be time-stamped at the top of the log file.

Author: Chris Frishkorn

Version Release History
---
**February 7th, 2016  - v1.2.3-alpha   -** *Started work on issue #51 & issue #54.*

**February 7th, 2016  - v1.2.2-alpha   -** *Fixed heartbeat, now has zeros appened to log file (issue #49).*

**February 7th, 2016  - v1.2.1-alpha   -** *Fixed RTC reset problem with colorSelect and projectSelect (issue #48).*

**February 6th, 2016  - v1.2.0-release -** *Released version 1.2.*

**February 3rd, 2016  - v1.1.4-beta    -** *Added heartbeat to log file. (issue #36 & issue #44)*

**January 23rd, 2016  - v1.1.3-alpha   -** *Project select boundary condition fixed, timers made consistant across code (issue #32).*

**January 22nd, 2016  - v1.1.2-alpha   -** *Fixed timer accuracy, improved log format (issue #38 & issue #39).*

**January 10th, 2016  - v1.1.1-alpha   -** *Improved log format, changed timer to hh:mm:ss format (issue #34 & issue #27).*

**January 10th, 2016  - v1.1.0-alpha   -** *Added project notfication when pressing UP/DOWN buttons (issue #30).*

**January 7th, 2016   - v1.0.0-release -** *Released version 1.0.*