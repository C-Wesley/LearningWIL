# Global Navigation Satellite System 
We are using the sparkfun ZED F9P GNSS reciever.  

<a href="https://www.sparkfun.com/sparkfun-gps-rtk-sma-kit.html" target="_blank">Sparkfun GNSS SMA Kit</a>

Objectives:
-Use the Sparkfun GNSS Library to get location information

We used a MKR1310 microcontroller. Any 3.3v logic microcontroller should work. 

<a href="https://docs.arduino.cc/hardware/mkr-wan-1310/" target="_blank">MKR1310</a>

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

![Boards Manager](img/SAMD.png)
![Boards Manager](img/v3.png)

## u-blox ZED-F9P GNSS (I²C)

**What the Program Does**
- Initializes a ZED-F9P GNSS module
- Reads:
  - Latitude and longitude
  - Time and date
  - Satellites in view (SIV)
- Prints raw GNSS data to Serial

# Related Lab Notebook Entries

## No-Library-Challenege 
[Revisiting the ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/10_Revisiting_the_Sparkfun_GPS.pdf)

[Reading the Number of Available Bytes ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/23_Reading_the_Number_of_Available_Bytes_ZED-F9P.pdf)

[Notes on the UBX Protocol for the ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/24_Notes_on_the_UBX_Protocol_for_the_ZED-F9P.pdf)

[Reading the NAV-PVT on the ZED-F9P without a Library](https://c-wesley.github.io/projects/asv/Notebook_2/24a_reading_NAVPVT_without_library.pdf)

[Processing the NAV-PVT Message on the ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/26_Processing_the_NAV-PVT_Message_on_ZED-F9P.pdf)

[Using U-Center to Configure the ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/27_Uising_U-Center_to_Configure_the_ZED-F9P.pdf)

## SparkFun GNSS V3 Library 

[SparkFun ZED-F9P Data Logger](https://c-wesley.github.io/projects/asv/Notebook_1/9_Sparkfun_ZED-F9P_Logging.pdf)

[SparkFun GNSS with the Library](https://c-wesley.github.io/projects/asv/Notebook_2/14_Sparkfun_GNSS_with_the_library.pdf)


