##timeClock - v2.0.3-alpha
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
**October 30th, 2016  - v2.0.4-alpha   -** *Initial commmit (issue #101).*

**October 28th, 2016  - v2.0.3-alpha   -** *Fixed Uninitialized projects.txt File from rendering blank projects (issue #65).*

**October 27th, 2016  - v2.0.3-alpha   -** *Removed references to hex values for ASCII, moved timerState to the correct location.*

**October 26th, 2016  - v2.0.2-alpha   -** *Fixed Carriage Return LCD rendering problem (issue #92).*

**September 5th, 2016 - v2.0.1-alpha   -** *Added seconds to heartbeat resolution (issue #90).*

**August 23rd, 2016   - v2.0.0-release -** *Released version 2.0.*

- See GitHub commit history for full version tracking notes.
