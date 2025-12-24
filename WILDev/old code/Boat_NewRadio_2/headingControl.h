#ifndef HEADING_CONTROL_H
#define HEADING_CONTROL_H

  #include "shared.h"

  struct GPSPair
  {
      // Lat1 and Lon1 can be viewed as the "current position".
      // Lat2 and Lon2 can be veiwed as the "target position". 
      long lat1_L; // Lattitude and longigutes
      long lon1_L; // The _L indicates they are 
      long lat2_L; // long data types. Not 
      long lon2_L; // in decimal form yet. 
  };

  GPSPair myGPSPair;
   
  double getDistanceBetween(GPSPair &someGPSPoints)
  {
    // returns distance in meters between two positions, both specified
    // as signed decimal-degrees latitude and longitude. Uses great-circle
    // distance computation for hypothetical sphere of radius 6372795 meters.
    // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
    // Courtesy of Maarten Lamers. Edited by Wesley Cooke to work with the GPS Pair struct.
    
    double lat1   =  (double)someGPSPoints.lat1_L  / 10000000.0; // Convert lat and long to degrees
    double long1  =  (double)someGPSPoints.lon1_L  / 10000000.0;
    double lat2   =  (double)someGPSPoints.lat2_L  / 10000000.0;
    double long2  =  (double)someGPSPoints.lon2_L  / 10000000.0;
    double delta  = radians(long1-long2);
    double sdlong = sin(delta);
    double cdlong = cos(delta);
    lat1          = radians(lat1);
    lat2          = radians(lat2);
    double slat1  = sin(lat1);
    double clat1  = cos(lat1);
    double slat2  = sin(lat2);
    double clat2  = cos(lat2);
    delta         = (clat1 * slat2) - (slat1 * clat2 * cdlong);
    delta         = sq(delta);
    delta        += sq(clat2 * sdlong);
    delta         =  sqrt(delta);
    double denom  = (slat1 * slat2) + (clat1 * clat2 * cdlong);
    delta         = atan2(delta, denom);
    return delta * 6372795;
  }

  double getTargetHeading(GPSPair &someGPSPoints)
  {
    // returns course in degrees (North=0, West=270) from position 1 to position 2,
    // both specified as signed decimal-degrees latitude and longitude.
    // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
    // Courtesy of Maarten Lamers. Edited by Wesley Cooke to work with the GPS Pair struct.
    
    double lat1  =  (double)someGPSPoints.lat1_L  / 10000000.0; // Convert lat and long to degrees
    double long1 =  (double)someGPSPoints.lon1_L  / 10000000.0;
    double lat2  =  (double)someGPSPoints.lat2_L  / 10000000.0;
    double long2 =  (double)someGPSPoints.lon2_L  / 10000000.0;
    double dlon  = radians(long2-long1);
    lat1         = radians(lat1);
    lat2         = radians(lat2);
    double a1    = sin(dlon) * cos(lat2);
    double a2    = sin(lat1) * cos(lat2) * cos(dlon);
    a2           = cos(lat1) * sin(lat2) - a2;
    a2           = atan2(a1, a2);
    
    if (a2 < 0.0)
    {
      a2 += TWO_PI;
    }
    return degrees(a2);
  }

  double getHeadingError(double aHeading, double aTargetHeading)
  {
      // Computes the difference between aHeading and aTargetHeading. 
      // Returns a value between -1 and 1 representing how away the two headings are. 
  
      double error = aTargetHeading - aHeading;
  
      // Give the shortest turn distance from -180 to 180. 
      if (error >= 180)
      {
          error -= 360;
      }
      else if (error <= -180)
      {
          error += 360;
      }
  
      // Return error number between -1 and 1.
      return error/180.0;
  }

  double fixHeading(double aHeading)
  {
    if (aHeading < 0.0)
    {
      aHeading += 360.0;
    }
    else if (aHeading >= 360.0)
    {
      aHeading -= 360.0; 
    }
    return aHeading; 
  }
  
#endif
