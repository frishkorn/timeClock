##timeClock - v1.4.0-alpha
---
###An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.

*Arduino Uno, SD Shield, and RGB LCD Shield are required. There is a utility in the utility folder you can run once to set the RTC.*

timeClock has 6 projects that can be selected using the UP / DOWN buttons. Back-light color will change for each Project and Project Name will be displayed. Time and date will always be displaying on the Main Screen. To display the Project Name at any time, press LEFT button.

To start a timer press SELECT, the back-light will change red notifying you the timer is running. The project title, start time, and date will be logged to the SD card using a CSV file. Pressing SELECT again will stop the timer, the project title and stop time will be logged to the SD card in addition to the elapsed timer. Time in log file is in 24-hour format.

Device records a heartbeat every 10 seconds in case of loss of power. When a new log file is created, the heartbeat will be time-stamped at the top of the log file.

Author: Chris Frishkorn

Version Tracking
---
**February 27th, 2016 - v1.4.0-alpha   -** *Started work on adding Project Name functionality (issue #40).*

**February 11th, 2016 - v1.3.1-alpha   -** *Changed time-out constant to use pre-processor #define (issue #60).*

**February 10th, 2016 - v1.3.0-alpha   -** *Added LEFT button press Project Name notification, updated variables (issue #33).*

**February 9th, 2016  - v1.2.4-alpha   -** *Fixed RTC Error message and optimized heartbeat log-file write code (issue #55).*

**February 7th, 2016  - v1.2.3-alpha   -** *Fixed uninitialized heartbeat and updated sync interval (issue #51 & issue #54).*

**February 7th, 2016  - v1.2.2-alpha   -** *Fixed heartbeat, now has zeros appened to log file (issue #49).*

**February 7th, 2016  - v1.2.1-alpha   -** *Fixed RTC reset problem with colorSelect and projectSelect (issue #48).*

**February 6th, 2016  - v1.2.0-release -** *Released version 1.2.*

**February 3rd, 2016  - v1.1.4-beta    -** *Added heartbeat to log file. (issue #36 & issue #44)*

**January 23rd, 2016  - v1.1.3-alpha   -** *Project select boundary condition fixed, timers made consistant across code (issue #32).*

**January 22nd, 2016  - v1.1.2-alpha   -** *Fixed timer accuracy, improved log format (issue #38 & issue #39).*

**January 10th, 2016  - v1.1.1-alpha   -** *Improved log format, changed timer to hh:mm:ss format (issue #34 & issue #27).*

**January 10th, 2016  - v1.1.0-alpha   -** *Added project notfication when pressing UP/DOWN buttons (issue #30).*

**January 7th, 2016   - v1.0.0-release -** *Released version 1.0.*

- See GitHub commit history for full version tracking notes.