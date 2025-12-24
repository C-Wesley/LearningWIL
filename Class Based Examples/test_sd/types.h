#ifndef TYPES_H 
#define TYPES_H

    #include "config.h"
    
    // Structs are like arrays, but 
    // instead of requiring the same type
    // we can use different types.
    // Each element in the struct is called 
    // a "member".
    // To access memebers, we use 
    // StructName.MemberName  <- notice the '.' 

    /*===================================================
        Navigation / GPS DATA Structures and Classes

        GlobalPositioningData contains infomration from the GPS.  

        GlobalPositioning manages the ineraction with the module itself
        
        Point contains a latitude and longitude pair

        Date contains the date
      ===================================================*/

    class Point
    {
        // The point class contains a latitude and longitude. 
        // Given another point, it can calculate the distance to that point
        // and the required heading to reach that point. 
        public:
            long latitude;  // Latitude  degrees * 10e-6
            long longitude; // Longitude degrees * 10e-6
            
            // Default to 0 constructor
            Point()
            {
                latitude  = 0;
                longitude = 0;
            }

            // Pass in lat and lon values 
            Point(long lat, long lon)
            {
                latitude = lat; 
                longitude = lon; 
            }

            // The first const says that we won't modify the point that is named "other". 
            // The second const says that we won't modify *this* instance of point.
            double distanceTo(const Point& other) const 
            {
                // returns distance in meters between two positions, both specified
                // as signed decimal-degrees latitude and longitude. Uses great-circle
                // distance computation for hypothetical sphere of radius 6372795 meters.
                // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
                // Courtesy of Maarten Lamers. Modified for the Point class by Wesley Cooke 
    
                double lat1   =  (double)latitude / 10000000.0; // Convert lat and long to degrees
                double long1  =  (double)longitude  / 10000000.0;
                double lat2   =  (double)other.latitude  / 10000000.0;
                double long2  =  (double)other.longitude  / 10000000.0;
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

            double getTargetHeading(const Point& other) const
            {
                // returns course in degrees (North=0, West=270) from position 1 to position 2,
                // both specified as signed decimal-degrees latitude and longitude.
                // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
                // Courtesy of Maarten Lamers. Modified for the Point class by Wesley Cooke.
                
                double lat1  =  (double)latitude  / 10000000.0; // Convert lat and long to degrees
                double long1 =  (double)longitude  / 10000000.0;
                double lat2  =  (double)other.latitude  / 10000000.0;
                double long2 =  (double)other.longitude / 10000000.0;
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
    }; 

    struct Date 
    {
        uint16_t year; 
        uint8_t month; 
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    };

    struct GlobalPositioningData 
    {
        Date date;
        Point position; 

        uint8_t fixType;
        bool gnssFixOK; 
        bool headVehValid;
        bool invalid_llh;          
        uint8_t SIV;           // Satellites in View
        long groundSpeed;      // mm/s 
        long headingMotion;    // degrees * 1e-5
        long headingVehicle;   // degrees * 1e-5
    };

    /* =======================================================
        System State

         The navigation state contains information used
         to make decisions about how to control WIL's heading

         MotorState contians the speed of the motors
       =======================================================*/
    struct NavigationState 
    {
        double distanceToWaypoint;  // Meters
        double targetHeading;       // degrees 0-360
        double currentHeading;      // degrees 0-360
        double headingError;        // Normalized Error [-1, 1]
        double gpsError;            // Degrees, difference GPS vs heading sensor
        double averageGPSError;     // degrees, running average
        int waypointIndex;          // current waypoin index
        int numWaypoints;           // total number of waypoints 
    };

    /*======================================
        Sensor Data 
         EcoData contains the readings from 
         the atlas scientific sensors. 
      ======================================*/

    struct EcoData 
    {
        float DO;            // Dissolved Oxygen
        float ORP;           // Oxidation-Reduction Potential
        float pH;            
        float conductivity;  // Electrical Conductivity 
        float waterTemp;     // Temperature in Celcius
    };

    /* ===========================================================================
        Program Modes of Operation
    
        An enumeration (or enum) is a user defined data type that contains 
        a set of named integer constants. It is used to assign meaningful 
        names to integer values, which makes a program easy to read and maintain.
        https://www.geeksforgeeks.org/c/enumeration-enum-c/
       ===========================================================================*/  

    enum ProgramMode : uint8_t
    {
        REMOTECONTROL = 0,
        AUTONOMOUS    = 1,    
        TUGBOAT       = 2    
    };

    /* ===========================================================================
        Heading Average
        This struct is used to implement a circular array.
        This means that when the last index is reached, the array
        wraps back around to the front of the array.

        (So structs can also have method associated with them, which basically
        makes them a class, but we don't have to dig too deep here. I'm only
        doing this because these variables are always used with these methods)
    ===========================================================================*/  
    class HeadingAverage
    {
        private: 
            double sinValues[HEADING_AVERAGE_LIMIT];
            double cosValues[HEADING_AVERAGE_LIMIT];
            byte count;
            byte frontIndex;
            double sinSum;
            double cosSum;

        public: 
            HeadingAverage() 
            {
                reset();
            }
            void update(double angleDegrees)
            {
                // This method will update our average given a new value. 

                if (count >= HEADING_AVERAGE_LIMIT) 
                {
                    // Remove the oldest value
                    sinSum -= sinValues[frontIndex];
                    cosSum -= cosValues[frontIndex];
                } else 
                {
                    count++;
                }
                // Get the x and y components
                double sinVal = sin(radians(angleDegrees));
                double cosVal = cos(radians(angleDegrees));
                //Add the components to the sum
                sinSum += sinVal;
                cosSum += cosVal;
                // Keep the value for later
                sinValues[frontIndex] = sinVal;
                cosValues[frontIndex] = cosVal;
                // Increment our front
                frontIndex = (frontIndex + 1) % HEADING_AVERAGE_LIMIT;
            }

            // This method doesn't modify the object state, so we mark it as const.
            double get() const
            {
                if (count == 0) return 0.0;
                return degrees(atan2(sinSum, cosSum));
            }

            void reset()
            {
                count = 0;
                frontIndex = 0;
                sinSum = 0;
                cosSum = 0;
                for (int i = 0; i < HEADING_AVERAGE_LIMIT; i++) {
                    sinValues[i] = 0;
                    cosValues[i] = 0;
                }
            }
    };
    
#endif