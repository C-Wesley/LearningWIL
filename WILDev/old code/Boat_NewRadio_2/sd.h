#ifndef SD_H
#define SD_H

  #include <SD.h>
  #include <SPI.h>
  #include "shared.h "
  
  int SDChipSelect = 48;
  
  void setupSD()
  {
    pinMode(SDChipSelect, OUTPUT);
    if(!SD.begin(SDChipSelect))
    {
     // If it fails to begin, freeze.
     // Check that the card is properly inserted. 
     digitalWrite(LED_BUILTIN, LOW);
     //Serial.println("SD Card failed to begin.");
     while(1){}
    }    

    File outputfile = SD.open("GPSData.txt", FILE_WRITE); 
    outputfile.println("Time, fixType, gnssFixOk, headVehValid, "
                       "invalid_llh, latitude, longitude, SIV, groundSpeed, headingMotion, "
                       "headingVehicle, distanceBetween, targetHeading, "
                       "currentHeading, headingError, gpsError, averageGPSError, index");

    outputfile.close();
  }

  void logToSd()
  {
    File outputfile = SD.open("GPSData.txt", FILE_WRITE);

    // Time Stamp
    outputfile.print(myGPSData.month);          outputfile.print("/"); 
    outputfile.print(myGPSData.day);            outputfile.print("/"); 
    outputfile.print(myGPSData.year);           outputfile.print(" "); 
    outputfile.print(myGPSData.hour);           outputfile.print(":"); 
    outputfile.print(myGPSData.minute);         outputfile.print(":"); 
    outputfile.print(myGPSData.second);         outputfile.print(", ");

    // Validity Flags
    outputfile.print(myGPSData.fixType);        outputfile.print(", ");
    outputfile.print(myGPSData.gnssFixOK);      outputfile.print(", ");
    outputfile.print(myGPSData.headVehValid);   outputfile.print(", ");
    outputfile.print(myGPSData.invalid_llh);    outputfile.print(", ");

    // Data for Navigation 
    outputfile.print(myGPSData.lat);            outputfile.print(", ");
    outputfile.print(myGPSData.lon);            outputfile.print(", ");
    outputfile.print(myGPSData.SIV);            outputfile.print(", ");
    outputfile.print(myGPSData.groundSpeed);    outputfile.print(", ");
    outputfile.print(myGPSData.headingMotion);  outputfile.print(", ");
    outputfile.print(myGPSData.headingVehicle); outputfile.print(", ");
    outputfile.print(distanceBetween);          outputfile.print(", ");
    outputfile.print(targetHeading);            outputfile.print(", ");
    outputfile.print(currentHeading);           outputfile.print(", ");
    outputfile.print(headingError);             outputfile.print(", ");
    outputfile.print(gpsError);                 outputfile.print(", ");
    outputfile.print(averageGPSError);          outputfile.print(", "); 
    outputfile.print(waypointIndex);            outputfile.println();
    
    // Close the file
    outputfile.close();  
  }

  void parsePath()
  {
    File pathFile = SD.open("path.csv");

    int ln = 0;
    while(pathFile.available())
    {
      ln++;                // Starting at line number one
      char aline[50] = {};
      char data      = ' ';
      int index      = 0; 

      while (data != '\n')
      {
        data = pathFile.read();
        aline[index] = data;
        index++;
      }

      if (ln == 1)
      {
        Serial.println("Header of Path File: ");
        Serial.print(aline);
      }
      else 
      {
        char* tok = strtok(aline, ",");      // Read the first token
        int id    = atoi(tok);               // Save it as an int

        tok            = strtok(NULL, ",");  // Read the next token
        long longitude = atoi(tok);          // This is our longitude

        tok            = strtok(NULL, ",");  // Read the next token
        long latitude  = atol(tok);          // Save it as a long

        tok            = strtok(NULL, ",");  // We should have a null value now

        dLons[id-1] = longitude;
        dLats[id-1] = latitude;
      }
    }
    numWayPoints = ln-1; // A this point we have a the number of waypoints + 1 since there is the header line.
  }
#endif
