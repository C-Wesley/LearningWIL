# Class Based Examples

## Overview
- These examples are derived from subsystems used in the WIL (Water Is Life) autonomous surface vehicle. Reviewing them before modifying the main WIL codebase will make it easier to understand how the individual components fit together.
- The examples assume some familiarity with C++ classes. If object-oriented programming is new to you, it may be helpful to review introductory material on C++ classes before working through these files.
- Examples are grouped by intended functionality; some headers like `types.h` appear in multiple folders, and some examples build on concepts introduced in earlier ones.

### C++ Class & OOP Tutorials
- Object-oriented programming is a broad topic, and it is common to spend years gradually becoming more comfortable with it. For the purposes of this project, however, only a small and practical subset of OOP concepts is required. The goal here is not mastery, but building enough understanding to read, use, and modify the WIL code with confidence.

- https://paulmurraycbr.github.io/ArduinoTheOOWay.html

- https://www.geeksforgeeks.org/cpp/object-oriented-programming-in-cpp/

- https://www.geeksforgeeks.org/cpp/c-classes-and-objects/

- https://www.learncpp.com/cpp-tutorial/introduction-to-object-oriented-programming/

## Design Philosophy

The examples in this repository follow a few guiding principles:

- **Separation of concerns**  
  Data definitions (`types.h`) are kept separate from logic and hardware interaction.

- **Self-contained modules**  
  Each test folder can be compiled and understood independently.

- **Clarity over cleverness**  
  Code prioritizes readability and learning value over extreme optimization.

## Recommended Learning Order
1. test_imu
2. test_gps
3. test_sensors
4. test_motor
5. test_sd
6. test_radio

## Typical layout 
| Path | Description |
| ---  | --- | 
| /test_functionality     | Contains code for \[functionality\]            |
|  &emsp;- types.h                | local copy of shared types                     |
|  &emsp;- config.h               | local copy of configuration variables          |
|  &emsp;- functionality.h        | Header file that contains the class definition |
|  &emsp;- test_functionality.ino | Example of using the class                     |

## What are structures and classes? 
A structure is a collection of like or related variables under some name. Structures don't have methods defined inside of them, but a function can be written that takes in a structure. 

A class is a collection of variables and their associated methods together. In pure C programming, you only have structures. But in C++, you have access to classes. 

## Purpose of `types.h`
- Defines common types, typedefs, and shared structs and classes used across examples.
- Serves to keep examples self-contained so each module can compiled independently.
- Classes like `Point` and `HeadingAverage` encapsulate math that would otherwise be duplicated.
- Avoid modifying `types.h` inside individual test folders unless explicitly experimenting. 
- Do not mix degrees and radians — all public interfaces use degrees.

## Explanation of Types

### `class Point`

The `Point` class represents a geographic location defined by latitude and longitude.

It also encapsulates navigation-related computations, such as:
- Distance between two points
- Required heading to reach another point

#### Members

| Member | Type | Description |
|------|------|-------------|
| `latitude` | `long` | Latitude in degrees × 10⁻⁶ |
| `longitude` | `long` | Longitude in degrees × 10⁻⁶ |

> Using a long data type avoids floating-point errors and is commonly used in GPS systems.

#### Constructors

- **Default constructor**  
  Initializes the point at `(0, 0)`.

- **Parameterized constructor**  
  Accepts latitude and longitude directly.

#### Methods

##### `double distanceTo(const Point& other) const`

Computes the great-circle distance (in meters) between this point and another point using spherical Earth geometry.

- Uses Earth radius: 6,372,795 meters
- Does not modify either point

##### `double getTargetHeading(const Point& other) const`

Computes the heading (bearing) required to travel from this point to another.

- Output range: \[0, 360\) degrees
- North = 0°, East = 90°, South = 180°, West = 270°
---
### `struct Date`

Represents a timestamp, typically sourced from the GPS module.

#### Members

| Member | Type | Description |
|------|------|-------------|
| `year` | `uint16_t` | Year |
| `month` | `uint8_t` | Month |
| `day` | `uint8_t` | Day |
| `hour` | `uint8_t` | Hour |
| `minute` | `uint8_t` | Minute |
| `second` | `uint8_t` | Second |
---
### `struct GlobalPositioningData`

Aggregates all GPS-related data into a single structure.

This struct is typically populated directly from the GNSS module and used by navigation and control logic.

#### Members

| Member | Type | Description |
|------|------|-------------|
| `date` | `Date` | GPS-provided timestamp |
| `position` | `Point` | Current latitude and longitude |
| `fixType` | `uint8_t` | GNSS fix type |
| `gnssFixOK` | `bool` | Valid GNSS fix flag |
| `headVehValid` | `bool` | Vehicle heading validity |
| `invalid_llh` | `bool` | Invalid lat/long/height flag |
| `SIV` | `uint8_t` | Satellites in view |
| `groundSpeed` | `long` | Ground speed (mm/s) |
| `headingMotion` | `long` | Motion heading (degrees × 10⁻⁵) |
| `headingVehicle` | `long` | Vehicle heading (degrees × 10⁻⁵) |
---
### `struct NavigationState`

Represents the current navigation and control state of WIL.

#### Members

| Member | Type | Description |
|------|------|-------------|
| `distanceToWaypoint` | `double` | Distance to target waypoint (meters) |
| `targetHeading` | `double` | Desired heading \[0–360°\) |
| `currentHeading` | `double` | Measured heading \[0–360°\) |
| `headingError` | `double` | Normalized heading error `[-1, 1]` |
| `gpsError` | `double` | Difference between GPS and heading sensor |
| `averageGPSError` | `double` | Running average of GPS error |
| `waypointIndex` | `int` | Current waypoint index |
| `numWaypoints` | `int` | Total number of waypoints |
---
### `struct EcoData`

Stores measurements from environmental sensors.

#### Members

| Member | Type | Description |
|------|------|-------------|
| `DO` | `float` | Dissolved Oxygen |
| `ORP` | `float` | Oxidation-Reduction Potential |
| `pH` | `float` | pH level |
| `conductivity` | `float` | Electrical conductivity |
| `waterTemp` | `float` | Water temperature (°C) |
---
### `enum ProgramMode`

Defines the operating mode of WIL.

#### Values

| Mode | Value | Description |
|----|------|-------------|
| `REMOTECONTROL` | `0` | Manual operator control |
| `AUTONOMOUS` | `1` | Fully autonomous navigation |
| `TUGBOAT` | `2` | Assisted or towing behavior |
---
### `class HeadingAverage`

Implements a circular buffer and vector averaging for averaging heading angles.

Because angles wrap around (e.g., 359° → 0°), a simple arithmetic mean does not work.  
This class instead averages sine and cosine components, producing a correct circular mean.

#### Methods

- `update(double angleDegrees)` — Adds a new heading sample
- `get()` — Returns the averaged heading (degrees)
- `reset()` — Clears internal state

## Build & usage
- Each module can be built independently. Connect the appropriate hardware.
