# -*- coding: utf-8 -*-
"""
Created on Sun Mar 30 16:01:53 2025

@author: Wesley
"""

import math
from math import radians, atan2, sin, cos, sqrt, degrees

def distanceBetween(lat1_L, lon1_L, lat2_L, lon2_L):
    
    lat1   =    lat1_L  / 10000000.0
    long1  =  lon1_L  / 10000000.0
    lat2   =  lat2_L  / 10000000.0
    long2  =  lon2_L  / 10000000.0
    delta  = radians(long1-long2)
    sdlong = sin(delta)
    cdlong = cos(delta)
    lat1   = radians(lat1)
    lat2   = radians(lat2)
    slat1  = sin(lat1)
    clat1  = cos(lat1)
    slat2  = sin(lat2)
    clat2  = cos(lat2)
    delta         = (clat1 * slat2) - (slat1 * clat2 * cdlong)
    delta         = delta**2
    delta        += (clat2 * sdlong)**2
    delta         =  sqrt(delta)
    denom  = (slat1 * slat2) + (clat1 * clat2 * cdlong)
    delta         = atan2(delta, denom)
    return delta * 6372795;

def headingBetween(lat1_L, lon1_L, lat2_L, lon2_L):
    
    lat1  =  lat1_L 
    long1 =  lon1_L  
    lat2  =  lat2_L  
    long2 =  lon2_L  
    dlon  = radians(long2-long1)
    lat1         = radians(lat1)
    lat2         = radians(lat2)
    a1    = sin(dlon) * cos(lat2)
    a2    = sin(lat1) * cos(lat2) * cos(dlon)
    a2           = cos(lat1) * sin(lat2) - a2
    a2           = atan2(a1, a2)
    
    if (a2 < 0.0):
      a2 += 2*math.pi
    
    return degrees(a2)
    