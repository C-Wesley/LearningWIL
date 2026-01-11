# ZED-F9P no library challenge
This example is a little bit complex. This took me several weeks to peice together.

# Purpose of this sketch
This sketch polls the ZED-F9P gnss receiver using the UBX binary protocol.
Specifically, we request the NAV-PVT message and parse the data it sends. 

This example exposes just a small glimps of all the heavy lifting that the SparkFun library is doing. 
Actually, the SparkFun library is MUCH more complicated than what is shown here. But we have succeeded in warding away all those wolves! 

# The readNavPVT() Function
- sends an empty UBX NAV-PVT poll message
- requests the response packet from the GNSS 
- Verifies the header, message type, length, and checksum
- Extracts data directly into our NavData struct. 

# Limitations
This code assumes valid responses. No check for an error or any timeout handling is included.
Use this code with extreme caution and care. If the GNSS does not respond, responds late, or returns fewer bytes than expected, the code will still try to parse the data. The code assumes the first byte read is the start of a UBX packet. If the I²C buffer is misaligned or contains leftover bytes, parsing will fail.

# Note book entires for more information: 

[Reading the Number of Available Bytes](https://c-wesley.github.io/projects/asv/Notebook_2/23_Reading_the_Number_of_Available_Bytes_ZED-F9P.pdf)
[Notes on the UBX Protocol for the ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/24_Notes_on_the_UBX_Protocol_for_the_ZED-F9P.pdf)
[Processing the NAV-PVT Message on the ZED-F9P](https://c-wesley.github.io/projects/asv/Notebook_2/26_Processing_the_NAV-PVT_Message_on_ZED-F9P.pdf)