#ifndef AVERAGE_H
#define AVERAGE_H

  // Limit our average to 100 values.
  const int limit      = 100; 
  byte      count      = 0; // How many values are currently in the average
  byte      frontIndex = 0; // Where in the array are we?
  double    sinSum     = 0; // Keep up with the y components
  double    cosSum     = 0; // Keep up with the x components 
  
  double sinValues[limit];  // This is where we will store all the y-components.
  double cosValues[limit];  // This is where we will store all the x-components. 

  void updateAverage(double aValue)
  {
    // This method will update our average given a new value. 

    // If we are full
    if (count >= limit)
    {
      // Remove the first value
      sinSum -= sinValues[frontIndex]; 
      cosSum -= cosValues[frontIndex]; 
    }
    else
    {
      // Increment our count
      count++; 
    }
    
    // Get the x and y components
    double sinValue = sin(radians(aValue));
    double cosValue = cos(radians(aValue));

    // Add the components to the sum
    sinSum += sinValue; 
    cosSum += cosValue; 

    // Keep the value for later
    sinValues[frontIndex] = sinValue; 
    cosValues[frontIndex] = cosValue;

    // Increment our front
    frontIndex = (frontIndex + 1) % limit;
  }

  double getAverage()
  {
    // If we haven't saved any values yet.  
    if (count == 0)
    {
      // Then return 0
      return 0.0;
    }
    else
    {
      // Otherwise return the atan2(y/x); 
      return degrees(atan2(sinSum, cosSum));
    }
  }

  void resetAverage()
  {
    count      = 0;
    frontIndex = 0;
    sinSum     = 0;
    cosSum     = 0;

    for (int i=0; i<limit; i++)
    {
      sinValues[i] = 0;
      cosValues[i] = 0;
    }
  }
#endif
