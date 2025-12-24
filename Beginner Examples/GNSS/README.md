# Global Navigation Satellite System 
We are using the sparkfun ZED F9P GNSS reciever.  

<a href="https://www.sparkfun.com/sparkfun-gps-rtk-sma-kit.html" target="_blank">Sparkfun GNSS SMA Kit</a>

Objectives:
-Use the Sparkfun GNSS Library to get location information

We used a MKR1310 microcontroller. Any 3.3v logic microcontroller should work. 

<a href="https://docs.arduino.cc/hardware/mkr-wan-1310/" target="_blank">Sparkfun GNSS SMA Kit</a>

Connections for using I2C:

| MKR1310           |Sparkfun GNSS       | 
|---------------    |--------------------|
| 3.3v              | 3.3v               | 
| GND               | GND                |
| SDA               | SDA                |
| SCL               | SCL                |

To install the boards manager for the 1310, go to tools -> board -> board manager. 
Search "SAMD" and look for the arduino SAMD boards.

To install the library go to tools -> manage libraries
Search "SparkFun GNSS v3".

![Boards Manager](https://github.com/C-Wesley/LearningWIL/blob/main/GNSS/img/SAMD.png)
![Boards Manager](https://github.com/C-Wesley/LearningWIL/blob/main/GNSS/img/v3.png)
