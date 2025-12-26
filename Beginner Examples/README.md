# Beginner Examples

This folder contains stand-alone Arduino sketches intended to introduce different functionalities of WIL through examples. Each program focuses on one idea at a time to help you learn all the different components of WIL. Here, I am not using classes or object oriented programming because it is easier to learn that way. 
  
These examples are meant to give you understanding of how each subsystem of WIL works as well as build your skills with communication protocols on arduino like I2C, UART, and SPI. 

I recommend building the circuits, typing the code from scratch, and experimenting with each sensor that you use. Typing the code from scratch will help you understand better and aid when reading the entire code base of WIL later on. 

As WIL grew into a more complex system, writing code in the style of these examples became difficult to maintain and extend. To address this, the project transitioned to using classes, which allowed functionality to be separated into well-defined components and enabled data and system state to be encapsulated more cleanly. However, starting here is crucial to your understanding of how the components work and provides a place to experiment, make mistakes, and learn before moving on to more complex programs.

In general, the development approach used throughout this project is to isolate and understand one piece of the system at a time before integrating it into the larger whole. These examples are a direct result of learning how to use individual sensors, peripherals, and interfaces in isolation prior to combining them into a complete system. 

## Atlas Scientific Sensors (I²C)

**What the Program Does**
- Scans the I²C bus to find connected devices
- Sends read commands (`'R'`) to:
  - Dissolved Oxygen (DO)
  - ORP
  - pH
  - Conductivity sensors
- Reads and parses floating-point measurements
- Prints all sensor values to the Serial Monitor

## u-blox ZED-F9P GNSS (I²C)

**What the Program Does**
- Initializes a ZED-F9P GNSS module
- Reads:
  - Latitude and longitude
  - Time and date
  - Satellites in view (SIV)
- Prints raw GNSS data to Serial

## SD Card Write & Read (SPI)

### Example 1: Writing and Reading a File

**What the Program Does**
- Initializes an SD card
- Deletes an existing file if present
- Writes text and numeric data to `data.txt`
- Reopens the file and prints its contents to Serial
---

### Example 2: Reading and Parsing a CSV File

**What the Program Does**
- Opens a CSV file (`path.csv`)
- Reads one line at a time
- Parses ID, longitude, and latitude fields
- Prints parsed data to Serial

## Servo / Electronic Speed Control (ESC) Motor Control (PWM)

**What the Program Does**
- Attaches a servo/ESC to a PWM pin (The ESC uses the exact same signal as a Servo) 
- Sends an 'off' signal for ESC startup
- Reads motor speed commands from Serial
- Sets motor speed