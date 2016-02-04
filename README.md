##timeClock - v1.2.0-alpha
---
###An Arduino driven time clock with 16x2 multi-color LCD display, user input buttons, RTC, and SD card.

*Arduino Uno, SD Shield, and RGB LCD Shield are required. There is a utility in the utility folder you can run once to set the RTC.*

timeClock has 6 projects that can be selected using the UP / DOWN buttons. Back-light color will change for each Project. Time and date will always be displaying on the main screen.

To start a timer press SELECT, the back-light will change red notifying you the timer is running. The project title, start time, and date will be logged to the SD card using a CSV file. Pressing SELECT again will stop the timer, the project title and stop time will be logged to the SD card in addition to the elapsed timer.

Author: Chris Frishkorn

Version Release History
---
**February 3rd, 2016  - v1.2.0-alpha   -** *Added heartbeat to log file. (issue #36 & issue #44)*

**January 23rd, 2016  - v1.1.3-alpha   -** *Project select boundary condition fixed, timers made consistant across code (issue #32).*

**January 22nd, 2016  - v1.1.2-alpha   -** *Fixed timer accuracy, improved log format (issue #38 & issue #39).*

**January 10th, 2016  - v1.1.1-alpha   -** *Improved log format, changed timer to hh:mm:ss format (issue #34 & issue #27).*

**January 10th, 2016  - v1.1.0-alpha   -** *Added project notfication when pressing UP/DOWN buttons (issue #30).*

**January 7th, 2016   - v1.0.0-release -** *Released version 1.0.*

**January 5th, 2016   - v0.4.1-beta    -** *Filenames now contain creation date from RTC date / time (issue #3).*

**January 3rd, 2016   - v0.4.0-alpha   -** *Added project memory, when device resets last project will be loaded (issue #6).*

**January 3rd, 2016   - v0.3.3-alpha   -** *Issue fixed to prevent user from leaving project once timer begins (issue #18).*

**December 29th, 2015 - v0.3.2-alpha   -** *Fixed error messages, LCD now displays errors (issue #17).*

**December 28th, 2015 - v0.3.1-alpha   -** *Optimized code around timers, removed RTC date time set functions (issue #10).*

**December 27th, 2015 - v0.3.0-alpha   -** *Added select Project ability with notification (issue #4).*

**December 27th, 2015 - v0.2.1-alpha   -** *Added Start / Stop LCD notification (issue #13).*

**December 23rd, 2015 - v0.2.0-alpha   -** *Start / Stop timer added to SELECT button (issue #2).*

**December 22nd, 2015 - v0.1.4-alpha   -** *Fixed when time is equal to 10 AM or 11 AM a 0 is preceded before the hours digit (issue #8).*

**December 21st, 2015 - v0.1.3-alpha   -** *Removed useless debouncing and delay, discovered library handles it internally.*

**December 21st, 2015 - v0.1.2-alpha   -** *Updated data-types across code and removed ECHO_TO_SERIAL debugging (#issue 5).*

**December 21st, 2015 - v0.1.1-alpha   -** *Added code to dump NVRAM from DS1307 on startup.*

**December 21st, 2015 - v0.0.1-alpha   -** *Code forked from arduinoTSens which runs the underlying RTC, LCD, and SD Arduino shields.*