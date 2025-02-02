# SD Card
These files show the basics of using the sd card. 
The main objectives here are to:
- Initalize SD Card
- Handle SD errors
- Write to a file
- Determine if a file already exists
- Delete a file if it exists
- Extract data from a file into appropriate data types

Begin by looking at SD_Card_RW. 
Then move onto path_parse.

An advanced user may find the SD class and File class API more useful (Located near the bottom of the page): 

<a href="https://docs.arduino.cc/libraries/sd/#SD%20class" target="_blank">SD Class API</a>

<a href="https://docs.arduino.cc/libraries/sd/#File%20class" target="_blank">SD Class API</a>

The following connections need to be made to an appropriate microcontroller: 
For the purpose of this demonstration, we are using a 3.3v microsd card reader, so we opted for the arduino mkr1310 board for it's 3.3v logic. Chip select may be any unused digital pin. 

|SD Card| MKR1310 |
|-------|---------|
|MOSI   | MOSI    |
|MISO   | MISO    |
|SCK    | SCK     |
|CS     | 5       |
|PWR    | VCC     | 
|GND    | GND     | 

![Example Connections](https://github.com/C-Wesley/LearningWIL/blob/main/SD_Card/img/sd_example.jpg)
Figure 1. Example connections to the MKR 1310 board. 
