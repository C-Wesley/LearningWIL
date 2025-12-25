# Class Based Examples

## Overview
- A collection of class-based C++ examples and supporting headers used for learning object oriented programming.
- Examples are grouped by intended functionality; some headers like `types.h` appear in multiple folders. Some files utilize some of the previous works. 

## Recommened Learning Order
1. test_imu
2. test_gps
3. test_sensors
4. test_motor
5. test_sd
6. test_radio

## Typical layout 
| Path | Description |
| ---  | --- | 
| - /test_functionality     | // Contains code for \[functionality\]            |
|  - types.h                | // local copy of shared types                     |
|  - config.h               | // local copy of configuration variables          |
|  - functionality.h        | // Header file that contains the class definition |
|  - test_functionality.ino | // Example of using the class                     |

## Purpose of `types.h`
- Defines common types, typedefs, and shared structs and classes used across examples.
- Serves to keep examples self-contained so each module can compiled independently.

## What are structures and classes? 
A structure is a collection of like or related varaibles under some name. Structures don't have methods defined inside of them, but a funciton can be writen that takes in a structure. 

A class is a collection of variables and their associated methods together. In pure C programming, you only have structures. But in C++, you have access to classes. 

## Build & usage
- Each module can be built independently. Connect the appropriate hardware.