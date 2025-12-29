# SD Logging and Path Parsing

This test tests two closely related utilities for WIL: 

1. SDLog Class - a SD car logging interface for GPS, navigation, and water quality data 

2. Path Class - A CSV based waypoint loader for autonomous navigation

These classes handle loading where the boat should go from an SD card path and logging what the boat did. 

## SDLog Class

SDlog is responsible for initializing the SD card and writing the logs. 

| File        | Purpose |
| ---         | ---     | 
| GPSData.csv | GPS and Navigation data during autonomous operation |
| ECO.csv     | Water quality data during remote control | 
| ECOR.csv    | Water quality data during remote control | 

Each file is created if missing and given a header on start. If the file already exists, a new header is appended and new data doesn't overwrite old data. 

### Constructor 
```cpp
SDLog myLog(uint8_t cs)
``` 
You have to provide the Chip Select pin as an input. 

### Changing file names
The file names can be changed to be different. I set them in config.h. 

```cpp
// config.h 
constexpr const char* GPS_DATA_FILE   = "GPSData.csv";
constexpr const char* ECO_DATA_FILE   = "ECO.csv"; 
constexpr const char* ECO_REMOTE_FILE = "ECOR.csv";

// inside main code
myLog.setAutoWaterDataFileName(ECO_DATA_FILE);
myLog.setGPSDataFileName(GPS_DATA_FILE);
myLog.setRemoteWaterDataFileName(ECO_REMOTE_FILE);
``` 

If the SD card isn't creating the file, try making the file name shorter. Don't use spaces and special characters. The SD card is picky with the file names. 

### begin() method 
```cpp
bool success = myLog.begin();
```

- Initializes the SD card
- Creates CSV files if needed
- Writes headers to each file 
- Returns false if SD initialization fails

**SDLog must be initialized before using the Path class.**

### logWaterData() method 
```cpp 
void logWaterData(const Date &date, const Point &location, const EcoData &data, ProgramMode mode) 

myLog.logWaterData(date, position, ecoData, mode);
```
The logWaterData method will select the proper file to log to based on the mode that is passed. 

### logAutonomous() method
```cpp
void logAutonomous(const NavigationState &state, const GlobalPositioningData &data)

myLog.logAutonomous(navState, gpsData);
``` 

Logs the autonomous navigation information including headings, heading errors, gps data, distances to waypoints, etc. Everything member of NavigationState struct and GlobalPositioningData struct are logged. 

## Path Class 

The Path class loads a list of waypoints from a CSV file on the SD card. 

### Expected format
```csv
id,longitude,latitude
1,-830000000,330000000
2,-830000100,330000100
3,-830000200,330000200
```

Coordinates should be in the long format. Not decimal. 
The first line is always treated as a header. The waypoint ID starts at 1. 

### Constructor 
```cpp
Path(int maxLength)
``` 

The maxLength argument sets how big of an array we allocate. I have been configuring this by changing the MAX_WAYPOINTS variable in config.h. 

:TODO: Read the path file to first determine the exact size necessary to allocate for the waypoint arrays. Don't be lazy like me. 

### Parsing the path
```cpp
bool success = myPath.parsePath();
``` 

- Opens 'path.csv' :TODO: Make this not hardcoded
- Loads waypoints into preallocated arrays of length MAX_WAYPOINTS. 
- Returns false if there is some sort of file error. Make sure the file extension is correct and the SD card is inserted. 

### Checking the path over serial monitor 
```cpp
myPath.printPath()
``` 
Prints all loaded waypoints to Serial. Useful for validation before deployment. 

## test_sd.ino
This sketch makes fake GPS, navigation, and sensor data so that you can just test the SD card functionalities without the other sensors connected. Additionally, you can test that each program mode opens the proper file. 