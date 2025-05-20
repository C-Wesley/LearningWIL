#ifndef DISPLAY_H 
#define DISPLAY_H 

  // Include the necessary libraries. 
  // The Adafruit GFX provides common things like circles and shapes.
  // The 1305 is specific to the hardware. Both are necessary. 
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1305.h>
  #include <SPI.h>
  #include "shared.h"

  // Display Object. We have 128 pixels on the x-axis, 64 pixels on the y-axis.
  // MOSI, and SCK are connected Normally.
  // D0 on the display is SCK
  // D1 on the display is MOSI
  // MISO is not used. 
  // Pin 11 is connected to D/C.
  // Pin  7 is connected to Reset.
  // Pin  6 is connected to CS. 
  // Use a 220 uf capacitor on the power rail.
  Adafruit_SSD1305 display(128, 64, &SPI, 11, 7, 6, 7000000UL); 

  // This is a bitmap of Dr. Hauger's Face :)
  static const unsigned char PROGMEM hauger_bmp[] =
  {   0xff, 0x80, 0x19, 0xff, 0xff, 0xff, 
      0xff, 0x38, 0x40, 0xff, 0xff, 0xff, 
      0xfe, 0x1f, 0xfe, 0x0f, 0xff, 0xff, 
      0xfe, 0x3b, 0xff, 0xf3, 0xff, 0xff, 
      0xf8, 0x67, 0xff, 0xf8, 0xff, 0xff, 
      0xf8, 0xe7, 0xc0, 0x3c, 0x7f, 0xff, 
      0xf1, 0xfc, 0x7f, 0xc1, 0x7f, 0xff, 
      0xf1, 0xf8, 0xff, 0xfd, 0x7f, 0xff, 
      0xe3, 0xf9, 0xff, 0xfe, 0x7f, 0xff, 
      0xc7, 0xf9, 0xff, 0xfe, 0x7f, 0xff, 
      0xc7, 0xf0, 0xff, 0xff, 0x7f, 0xff, 
      0xc7, 0xf0, 0xff, 0xff, 0xbf, 0xff, 
      0xc7, 0xf0, 0x7f, 0xff, 0xbf, 0xff, 
      0xc7, 0xf0, 0xff, 0xff, 0xbf, 0xff, 
      0xcf, 0xf0, 0x3f, 0xff, 0xbf, 0xff, 
      0xcf, 0xf0, 0x7f, 0xfc, 0x1f, 0xff, 
      0xcf, 0xc0, 0x07, 0xf8, 0x1f, 0xff, 
      0xce, 0x00, 0x01, 0xfb, 0xcf, 0xff, 
      0xce, 0x00, 0x01, 0xf0, 0x6f, 0xff, 
      0xcc, 0x00, 0x00, 0x70, 0x27, 0xff, 
      0xcc, 0x00, 0x00, 0x07, 0x27, 0xff, 
      0xc0, 0x18, 0x38, 0x30, 0x27, 0xff, 
      0xc0, 0xf0, 0x00, 0x31, 0xf7, 0xff, 
      0xcf, 0x80, 0x00, 0x3e, 0xf7, 0xff, 
      0xd8, 0x00, 0x00, 0x3f, 0xf7, 0xff, 
      0xc0, 0x01, 0x80, 0x3f, 0xf7, 0xff, 
      0xc0, 0x01, 0xfc, 0x1f, 0xf7, 0xff, 
      0xc0, 0x00, 0x7c, 0x07, 0xf7, 0xff, 
      0xc0, 0x00, 0x00, 0x00, 0xf7, 0xff, 
      0x80, 0x00, 0x00, 0x03, 0x77, 0xff, 
      0x80, 0x00, 0x00, 0x0f, 0xf7, 0xff, 
      0xa0, 0x00, 0x00, 0x0f, 0xf7, 0xff, 
      0xd8, 0x00, 0x00, 0x1f, 0xf7, 0xff, 
      0xc0, 0x00, 0x00, 0x1f, 0xf7, 0xff, 
      0xf0, 0x00, 0x00, 0x00, 0x77, 0xff, 
      0xf8, 0x00, 0x00, 0x00, 0x77, 0xff, 
      0xf8, 0x00, 0x10, 0x07, 0xe7, 0xff, 
      0xf8, 0x00, 0x08, 0x1f, 0xe7, 0xff, 
      0xf8, 0x00, 0x08, 0x03, 0xe7, 0xff, 
      0xf8, 0x00, 0x00, 0x07, 0xe7, 0xff, 
      0xf8, 0x00, 0x00, 0x3f, 0xe7, 0xff, 
      0xf8, 0x00, 0x00, 0xff, 0xe7, 0xff, 
      0xfa, 0x00, 0x00, 0x7f, 0xc7, 0xff, 
      0xfb, 0x80, 0x00, 0x0f, 0xc7, 0xff, 
      0xf8, 0xe0, 0x00, 0x03, 0x8f, 0xff, 
      0xf8, 0x38, 0x00, 0x01, 0x9f, 0xff, 
      0xf8, 0x0c, 0x00, 0x00, 0x1f, 0xff, 
      0x00, 0x03, 0x00, 0x00, 0x3f, 0xff, 
      0x0e, 0x01, 0x80, 0x00, 0x1f, 0xff, 
      0x01, 0xe0, 0x60, 0x00, 0x07, 0xff, 
      0x00, 0x38, 0x30, 0x00, 0x73, 0xff, 
      0x00, 0x0e, 0x18, 0x00, 0xf1, 0xff, 
      0x00, 0x03, 0x86, 0x01, 0xf0, 0xff, 
      0x0e, 0x00, 0xe3, 0x03, 0xf0, 0x7f, 
      0x3b, 0x80, 0x31, 0xc1, 0xe1, 0x3f, 
      0x60, 0xc0, 0x1c, 0x60, 0xe1, 0x9f, 
      0x40, 0x60, 0x06, 0x30, 0xe0, 0x8f, 
      0x40, 0x30, 0x03, 0x18, 0x60, 0x87, 
      0x40, 0xe0, 0x01, 0x0c, 0xe0, 0xc3, 
      0x20, 0x80, 0x01, 0x84, 0xe0, 0x41, 
      0x30, 0x00, 0x00, 0x03, 0xe0, 0x41, 
      0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x0f
  };

  void setupDisplay()
  {
    if (!display.begin(0x3c))
    {
      Serial.println("Failed to set up display...");
      while(1){}
    }
    display.clearDisplay();
    // drawBitmap      x0, y0,    b itmap, width, height, color
    display.drawBitmap( 0,  0, hauger_bmp,    48,     64,    1);
    
    // Prepare and draw text 
    display.setTextSize(1); display.setTextColor(WHITE);
    display.setCursor(58, 14); display.println("Standby to");
    display.setCursor(61, 22); display.println("Get Some!");
    display.setCursor(50, 57); display.println("#GetObsessed!");
    
    // Text Box
    //                    x0, y0,  w,  h, r, color
    display.drawRoundRect(55, 12, 65, 19, 3,   1);
    //                    x0, y0, x1, y1, x2, y2, color
    display.fillTriangle( 50, 30, 55, 25, 55, 30,   1);
    display.drawPixel(56, 30, 1);
    display.display();
  }

  void displayRemoteInfo()
  {
    // Clear the Display. 
    display.clearDisplay(); 
    // Draw a border 
    //                    x0, y0,               w,                h, r, color
    display.drawRoundRect( 0,  0, display.width(), display.height(), 0, WHITE);
    display.setTextSize(1); display.setTextColor(WHITE);
    display.setCursor(2, 2); 
    
    display.print("MODE: ");
    display.println("REMOTE CONTROL");
    
    // Dipslay the Left and Right speed of the boat.
    display.setCursor(2, 10);
    display.print("LeftSpeed : " ); display.println(leftSpeed); 
    display.setCursor(2, 18);
    display.print("RightSpeed: "); display.println(rightSpeed);

    display.display(); 
  }

  void displayAutonomousInfo()
  {
    if (Serial1.available()>=28)
    {
      char data = Serial1.read();
      if (data == 'C')
      {
        char dummyArray[5];
        char dummyArray2[5];

        // Parse Waypoint Index
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read();
        } 
        dummyArray[4] = '\0';
        memcpy(&waypointIndex, dummyArray, sizeof(int));

        // Parse numWayPoints
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read();
        } 
        dummyArray[4] = '\0';
        memcpy(&numWayPoints, dummyArray, sizeof(int));

        // Parse distance 
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read();
          Serial.write(dummyArray[ii]);
        } 
        Serial.println();
        dummyArray[4] = '\0';
        
        // Parse C Heading
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read();
        } 
        dummyArray[4] = '\0';
        memcpy(&currentHeading, dummyArray, sizeof(float));

        // Parse T heading
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read();
        } 
        dummyArray[4] = '\0';
        memcpy(&targetHeading, dummyArray, sizeof(float));

        // Parse Lat 
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read();
        } 
        dummyArray[4] = '\0';
        memcpy(&lat, dummyArray, sizeof(long));

        // Parse lon
        for (int ii=0; ii<4; ii++)
        {
          dummyArray[ii] = Serial1.read(); 
        } 
        dummyArray[4] = '\0';
        memcpy(&lon, dummyArray, sizeof(long));

        if ((char)Serial1.read() != 'E')
        {
          Serial.println("Failed to parse properly...");
        }
      }
    }
    Serial.print(waypointIndex); Serial.print(" "); Serial.print(distanceBetween); Serial.print(" "); Serial.print(currentHeading); Serial.print(" "); Serial.print(lat); Serial.print(" "); Serial.println(lon); 
    
    // Clear the Display. 
    display.clearDisplay(); 
    // Draw a border 
    //                    x0, y0,               w,                h, r, color
    display.drawRoundRect( 0,  0, display.width(), display.height(), 0, WHITE);
    display.setTextSize(1); display.setTextColor(WHITE);
    display.setCursor(2, 2); 
    
    display.print("MODE: ");
    display.println("AUTONOMOUS");

    display.setCursor(2, 10);
    display.print("Waypoint: "); display.print(waypointIndex); display.print("/"); display.print(numWayPoints);

    display.setCursor(2, 18);
    display.print("Distance: "); display.print(distanceBetween);

    display.setCursor(2, 26);
    display.print("Heading: ");  display.print(currentHeading);

    display.setCursor(2, 34);    
    display.print("Lat: ");      display.print(lat);

    display.setCursor(2, 42);
    display.print("Lon: ");      display.print(lon);
    
    display.display(); 
  }
  
#endif
