# ZED-F9P with the Sparkfun GNSS library

## Close the blinds!! The wolves are gonna get ya!

# Purpose of Sketch
This sketch shows how to do the following: 
- Initialize a u-blox ZED-F9P GNSS module over I2C. 
- Read basic position, velocity, and time (PVT) data. 

# Notes
- Latitude and Longitude are returned as integers. Divide by 10^6 to get degrees. 
- Time and date are printed without leading zeros so be careful. 
- We have an arbitrary delay(1000). It is not necessary, but helps keep the output readable. 