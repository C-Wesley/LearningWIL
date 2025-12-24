#ifndef HEADING_CALCULATIONS_H
#define HEADING_CALCULATIONS_H

#include "../shared/types.h"
#include "../shared/config.h"

namespace Heading 
{
    double getDistanceBetween(const GPSPair &gpsPoints);
    double getTargetHeading(const GPSPair &gpsPoints);
    double getHeadingError(double currentHeading, double targetHeading);
    double fixHeading(double heading);
}

#endif