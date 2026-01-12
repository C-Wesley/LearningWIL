# Beginner Examples

This folder contains stand-alone Arduino sketches intended to introduce different functionalities of WIL through examples. Each program focuses on one idea at a time to help you learn all the different components of WIL. Here, I am not using classes or object oriented programming because it is easier to learn that way. 
  
These examples are meant to give you understanding of how each subsystem of WIL works as well as build your skills with communication protocols on arduino like I2C, UART, and SPI. 

I recommend building the circuits, typing the code from scratch, and experimenting with each sensor that you use. Typing the code from scratch will help you understand better and aid when reading the entire code base of WIL later on. 

# Recommended Learning Order
- basic_imu
- basic_gps
- basic_sd_card
- basic_thrusters
- basic_sensors
- basic_radio

Most examples have several levels of understanding. Take the GPS for instance. It has both a library version and a non library version. If your goal is to just use the data from the sensor, use the library to make it easy. If your goal is to better understand I2C, take a look at the 'no-library-challenge'. It's not for the faint of heart!

As WIL grew into a more complex system, writing code in the style of these examples became difficult to maintain and extend. To address this, the project transitioned to using classes, which allowed functionality to be separated into well-defined components and enabled data and system state to be encapsulated more cleanly. However, starting here is crucial to your understanding of how the components work and provides a place to experiment, make mistakes, and learn before moving on to more complex programs.

In general, the development approach used throughout this project is to isolate and understand one piece of the system at a time before integrating it into the larger whole. These examples are a direct result of learning how to use individual sensors, peripherals, and interfaces in isolation prior to combining them into a complete system. 
