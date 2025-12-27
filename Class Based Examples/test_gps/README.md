# GPS Class 

This folder contains a from-scratch GPS driver written in C++ for Arduino. It communicates directly with a u-blox ZED-F9P GPS module over I²C and parses raw binary data into values like latitude, longitude, speed, and heading. 

If you only want GPS data quickly, a library like in the beginner examples is easier to use. 

If you want to understand what the code the Sparkfun library is doing, this is a step in the right direction. The problem is that the official implementation relies on concepts like mutexes. Mutexes are powerful, an excellent tool, and absolutely not something I want to explain in this project.

So this implementation trades completeness and complexity for simplicity. It’s simpler, more readable, but also limited.

## Big Picture of the class

Every call of getData(); will do the following: 

1. Old GPS data is cleared from the I²C buffer. 
2. The GPS is polled for a NAV-PVT message. 
3. The code waits until the GPS has data or a timeout limit is reached. 
4. Assuming there is data, a packet is read from the GPS. 
5. The packet header and checksum are verified
6. Raw bytes are parsed into meaningful values 
7. A `GlobalPositioningData` object is updated 

If any step fails, the data is rejected. 

## File Breakdown 
- `gps.h` 
This files contains the GPS class called `GlobalPositioning`. 

- `test_gps.ino`
This file contains example code of using the class. 

- `types.h`
This file defines the GlobalPositioningData structure. 

- `config.h` 
Contains the I²C address we want to use.

Raw GPS bytes can be printed by enabling 
```constexpr bool DEBUG_PRINT_GPS_DATA = true;```

## The `GlobalPositioning` Class

### Constructor

```cpp
GlobalPositioning(uint8_t address); 
```

Creates a GPS interface using the given address. 

### Public Members 
```cpp
GlobalPositioningData data;
```
Holds the most recently parsed GPS data.

This structure is only updated when a complete, valid packet is received. If packet validation fails, the contents of data remain unchanged. 

---
```cpp
bool begin()
``` 
Checks that a device acknowledges on the I²C bus. 
Does not do any configuration. 

:TODO: extend begin() to configure the GPS too. 

Returns true if the device responds. 
Returns false if no acknowledgement is received.

Be mindful of the snake, young player.
This function does not verify that the responding device is actually the gps. It only verifies that something answered at the given I²C address.

If SneakySnakeSensor happens to share the same address, begin() will still return true. 

`begin()` confirms presence, not identity. 

---
```cpp
int getData();
```
Requests a NAV-PVT message from the GPS and attempts to parse it. 

This method will do the following:
- Clear any stale bytes from the GPS I²C buffer. 
- Polls the GPS until a full packet is available (or a timeout hits) 
- Verifies packet header and payload length
- Computes and checks the UBX checksum 
- Parses raw bytes into the `GlobalPositioningData` structure 

Will return 1 for new valid GPS data
Will return 0 for timeout, checksum fail, or invalid packet. 