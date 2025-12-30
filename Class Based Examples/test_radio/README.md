# Radio Module for remote control features

The radio.h file implements a simple UART radio communication protocol using the Sik Telemetry Radios. 

The Sik radio handles the actual transmission of data over the air, including packetization and error correction. From the perspective of the microcontroller, the radio behaves like a transparent UART link. All we need to do is send and receive bytes over a serial connection.
On top of this, a simple protocol is implemented using header and footer bytes. This helps keep messages synchronized and makes it easier to determine the intent of each packet when it is received.

The 'RadioControl' class is intended to run on the boat side of WIL and is responsible for: 
- Parsing incoming radio packets 
- Storing the most recent received commands or requests 
- Sending responses/data back to the remote control 

## Important design note 
The radio doesn't control the motors directly. Instead it provides received state variables that the rest of the system can act on.
This helps keep communication logic separate from control logic. 

## General Packet Structure 
All packets follow the same high-level format.

`[header][data][footer]`

| Name | Purpose |
|---   | ---     |
|header| Single character identifying packet type|
|data  | the actual message data that we care about. This could be a single byte or multiple depending on what is being sent. Generally it will be a specific struct. | 
| footer | Single character identifying the end of the packet. I use the character 'E' |

## Example of Packets the remote sends to the boat 

### Speed Packet
Asks the boat to go a certain speed. 

|             |                    |
|---          |---                 |
|Header       | 'S'                |  
|Data Byte 1  | `int leftMotorRec` |
|Data Byte 2  | `int rightMotorRec`|
|Footer       | 'E'                |

### Mode Packet 
Informs the boat of what the program mode should be. 

|             |                  |
|---          |---               |
|Header       | 'M'              |  
|Data Byte 1  | `char modeChar`  |
|Footer       | 'E'              |

|Mode Values |               | 
| ---        | ---           |
| '0'        | AUTONOMOUS    | 
| '1'        | REMOTE CONTROL| 
| '2'        | TUGBOAT       | 

The remote dictates the program mode. From the boat’s perspective, the rest of the system checks the `RadioControl` class to determine the current `ProgramMode`.


### Remote Reading Request

Sent from the remote control to the boat.
Signals that the remote control is requesting environmental data. 
Will set a variable called 'takeRemoteReading' to true so the boat knows it needs to take a reading and send it. 'takeRemoteReading' will be set back to false after the data is sent. 

Notice there is no data sent. We just send an 'R' in the header. 
|             |                  |
|---          |---               |
|Header       | 'R'              |  
|Footer       | 'E'              |

## Examples of Packets the boat sends to the remote

### EcoData Response
This happens when the flag 'takeRemoteReading' is true. The data is sent as raw bytes using the 

|             |                  |
|---          |---               |
|Header       | 'R'              |  
|Data Bytes  | EcoData ecoData  (this is one of our structs)|
|Footer       | 'E'              |

### NavigationState Response 

This is typically sent when in autonomous mode at some rate or at new waypoints. 

|             |                  |
|---          |---               |
|Header       | 'C'              |  
|Data Bytes  | NavigationState navState (this is one of our structs)    |
|Footer       | 'E'              |

## RadioControl Class Overview

Key public Members: 
- ProgramMode mode: Stores the program state
- int leftMotorRec: the left motor speed we want when in remote control mode. Dictated by the joystick on the controller. 
- int rightMotorRec: the right motor speed we want when in remote control mode. Dictated by the joystick on the controller. 
- bool takeRemoteReading: flag used to force the boat to take a reading of the sensors. Activated by pressing a button on the remote control. 

Key Methods: 
- begin(baud) - initializes the serial radio 
- read() - parse incoming packets 
- sendEcoData() - sends eco data back to the remote control
- sendAutoData - sends a NavigationState struct back to the remote control. 

read() should be called as frequently as possible. Don't write blocking methods or things that risk falling into a loop that can't clear. If this happens, read() will not be able to be called and you may lose control of WIL. 

## Examples

The examples in this folder are intended to be run simultaneously on two different microcontrollers, each connected to a Sik telemetry radio.

- `test_radio_controller` simulates a remote without requiring a physical controller.
- `test_radio_boat` simulates the boat's response to incoming packets.