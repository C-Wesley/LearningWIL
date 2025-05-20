#ifndef ECO_H
#define ECO_H

  #include <OneWire.h>
  #include <DallasTemperature.h>
  #include <Wire.h>
  #include "shared.h"

  #define ONE_WIRE_BUS 13       //whatever pin data is connected to
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  
  const int DOaddress  = 0x61;  // 97
  const int ORPaddress = 0x62;  // 98
  const int pHaddress  = 0x63;  // 99
  const int Caddress   = 0x64;  // 100
  
  char  DOdata[20];          
  char ORPdata[20];           
  char  pHdata[20];          
  char   Cdata[20];
  
  float DO;                 
  float ORP;                  
  float pH;                 
  float C;
  float tempC;
  
  byte code       = 0;  
  byte sensorCode = 0;  
  byte charIn     = 0;  
  byte jj         = 0;
  int  timeDelay  = 815;

  float sumDO;
  float sumORP;
  float sumpH;
  float sumC;
  float sumTemp; 

  float measureDO()
  {
    Wire.beginTransmission(DOaddress);
    Wire.write(82);
    Wire.endTransmission();
    delay(timeDelay);
  
    Wire.requestFrom(DOaddress, 20, 1);
    code = Wire.read();
  
    while (Wire.available())
    {
      charIn = Wire.read();
      DOdata[jj] = charIn;
      jj++;
      if (charIn == 0)
      {
        jj = 0;
        break;
      }
    }
    Wire.endTransmission();
    DO = atof(DOdata);
    charIn = 0;
    for (int jj = 0; jj < 20; jj++)
    {
      DOdata[jj] = 0;
    }
    return DO;
  } //end of measureDO()
  
  float measureORP()
  {
    Wire.beginTransmission(ORPaddress);
    Wire.write(82);
    Wire.endTransmission();
    delay(timeDelay);
    
    Wire.requestFrom(ORPaddress, 20, 1);
    code = Wire.read();
  
    while (Wire.available())
    {
      charIn = Wire.read();
      ORPdata[jj] = charIn;
      jj++;
      if (charIn == 0)
      {
        jj = 0;
        break;
      }
    }
    Wire.endTransmission();
    ORP = atof (ORPdata);
    charIn = 0;
    for (int jj = 0; jj < 20; jj++)
    {
      ORPdata[jj] = 0;
    }
    return ORP;
  } // end of measureORP()
  
  float measurepH()
  {
    Wire.beginTransmission(pHaddress);
    Wire.write(82);
    Wire.endTransmission();
    delay(timeDelay);
  
    Wire.requestFrom(pHaddress, 20, 1);
    code = Wire.read();
  
    while (Wire.available())
    {
      charIn = Wire.read();
      pHdata[jj] = charIn;
      jj++;
      if (charIn == 0)
      {
        jj = 0;
        break;
      }
    }
    Wire.endTransmission();
    pH = atof(pHdata);
    charIn = 0;
    for (int jj = 0; jj < 20; jj++)
    {
      pHdata[jj] = 0;
    }
    return pH;
  } //end of measurepH()
  
  float measureT()
  {
    sensors.requestTemperatures();
    delay(750);
    float tempC = sensors.getTempCByIndex(0);
    delay(1000);
    return tempC;
  }  //end measureT ()
  
  float measureC()
  {
    Wire.beginTransmission(Caddress);
    Wire.write(82);
    Wire.endTransmission();
    delay(timeDelay);
  
    Wire.requestFrom(Caddress, 20, 1);
    code = Wire.read();
  
    while (Wire.available())
    {
      charIn = Wire.read();
      Cdata[jj] = charIn;
      jj++;
      if (charIn == 0)
      {
        jj = 0;
        break;
      }
    }
    Wire.endTransmission();
    C = atof(Cdata);
    charIn = 0;
    for (int jj = 0; jj < 20; jj++)
    {
      Cdata[jj] = 0;
    }
    return C;
  }
  //end of measureC()

  void readEco()
  {
    currDO  = measureDO();
    currORP = measureORP();
    currpH  = measurepH();
    //tempC   = measureT();
    currC   = measureC();
  }

  void averageEco()
  {
    sumDO   = 0;
    sumORP  = 0;
    sumpH   = 0;
    sumC    = 0;
    sumTemp = 0;
    for(int ii=0; ii<5; ii++)
    {
      readEco();
      
      sumDO   = sumDO + currDO;
      sumORP  = sumORP+ currORP;
      sumpH   = sumpH + currpH;
      sumC    = sumC  + currC; 
      sumTemp = sumTemp + tempC; 
      delay(50);
    }

    averageDO  = sumDO/5.0;
    averageORP = sumORP/5.0;
    averagepH  = sumpH/5.0; 
    averageC   = sumC/5.0; 
    averageTemp= sumTemp/5.0; 

    
  }

#endif
