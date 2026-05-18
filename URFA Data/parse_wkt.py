# -*- coding: utf-8 -*-
"""
Created on Sun Mar 30 11:52:13 2025

@author: Wesley
"""

def parse_wkt_latlon(wkt_strings):
    """ 
    This method is designed to parse WKT POINT data and return 
    two lists containing the latitude and longitude values. 
    
    Assumptions: 
        wkt is non empty 
        
    Inputs:
        wkt (List):  List of strings containing POINT WKT data. 
                     One element of the list should looke like:
                         "POINT (-82.0054761 33.4925271)" 
                         
    Outputs: 
        longitude (List(float)): List of floats containing the longitude values
                                 corresponding to the WKT data. One element 
                                 of the list should looke like:
                                     -82.0054761
        latitude  (List(float)): List of floats containing the latitude values
                                 corresponding to the WKT data. One element 
                                 of the list should looke like:
                                      33.4925271
    """
    
    # Return Lists:
    latitude  = []
    longitude = []
    # Loop over the strings 
    for wkt in wkt_strings:
    
        # This line will first replace the '(' character with nothing.
        # then it will replace the ')' character with nothing.
        # The data will be a string and look like: "POINT -82.0054761 33.4925271"
        # Finally, we split the string on the spaces. 
        # This results in 3 columns of data that will be a list of 3 strings: 
        # ['POINT', '-82.0054761', '33.4925271']
        delimted = wkt.replace('(', '').replace(')', '').split(' ')

        # Extract the lattitude and Longitude values, convert them to floats, 
        # and place them into a new list
        latitude.append(float(delimted[1]))
        longitude.append(float(delimted[2]))
    
    # Return the populated lists
    return longitude, latitude 