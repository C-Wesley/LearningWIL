# -*- coding: utf-8 -*-
"""
Created on Sun Mar 30 11:54:01 2025

@author: Wesley
"""

# Used for loading the TIF file 
import rasterio  
import rasterio.plot

# Used for plotting.
import matplotlib.pyplot as plt

# Used for reading csv file and parsing data. 
import pandas as pd

# Method used to make calculations
from measurements import headingBetween
from math import radians, sin, sqrt

def plotPath(measuredDataFile: str, waypointsFile: str, xlim: list, ylim: list, title: str, plot)->float: 
    
    #tif_file = 'canal_modified.tif'
    tif_file  = 'BigLakeOlmstead.tif'
    
    # Open the measured data based on the filetype. 
    if measuredDataFile.endswith('.xlsx'):
        measuredData = pd.read_excel(measuredDataFile)
    elif measuredDataFile.endswith('.csv'):
        measuredData = pd.read_csv(measuredDataFile)
    else:
        raise Exception("Input files must be in the form of .xlsx or .csv")
    
    if waypointsFile.endswith('.xlsx'):
        wayData = pd.read_excel(waypointsFile)
    elif waypointsFile.endswith('.csv'):
        wayData = pd.read_csv(waypointsFile)
    else:
        raise Exception("Input files must be in the form of .xlsx or .csv")
    
    # Strip whitespace and set column names to lower case.
    measuredData = measuredData.rename(columns=lambda x: x.strip().lower())
    wayData      = wayData.rename(columns = lambda x: x.strip().lower())
    
    # Make sure the csv file has the proper headers
    if ('latitude' not in wayData.columns or 'latitude' not in measuredData.columns):
        raise Exception("A file does not contain the key: 'latitude'. ")
    
    if ('longitude' not in wayData.columns or 'longitude' not in measuredData.columns):
        raise Exception("A file does not contain the key: 'longitude'. ")
    
    # Filter out bad rows
    measuredData = measuredData.drop(measuredData[measuredData['gnssfixok'] !=1].index)
    measuredData = measuredData.drop(measuredData[measuredData['index'] == 0].index)
    
    # Grab the latitude and longitude data based on format of origin file
    ## :TODO: Make the file format match so this isn't needed.
    if measuredDataFile.endswith('.csv'):
        measured_lon_data = measuredData['longitude'] / 10**7
        measured_lat_data = measuredData['latitude'] / 10**7
    else:
        measured_lon_data = measuredData['longitude']
        measured_lat_data = measuredData['latitude']
        
    # Convert the waypoints from long to decimal  
    wayLat = wayData['latitude']  / 10**7
    wayLon = wayData['longitude'] / 10**7
    
    if plot:
        # Generate a matplotlib plot
        fig, ax1 = plt.subplots()
        ax1.clear() 
        
        # Create a list that has the same length as measured_lon.
        # This is used for the color map features
        dummyvars = list(range(0, len(measured_lon_data)))
        
        # Use a scatterplot for the measured data. 
        ax1.scatter(measured_lon_data,  \
                     measured_lat_data,  \
                                          s=2,  \
                        label="Path Traveled",  \
                                  c=dummyvars,  \
                                cmap='autumn')
            
        # Use a plot for the waypoints. This will connect the waypoints with a straigt line. 
        ax1.plot(wayLon,               \
                 wayLat,               \
                'c--',                 \
                 label="Desired Path", \
                 linewidth=1)
        
        # Legend uses the "label" argument form the scatter and plot feature. 
        #ax1.legend()
    
        # This ensures that the latitude and longitude pulled out and used as an offset. 
        # With out it, it may scale the lattitude to 3.3e1 and place 0.005... etc on the axis. 
        ax1.get_xaxis().get_major_formatter().set_useOffset(False)
        ax1.get_yaxis().get_major_formatter().set_useOffset(False)
        
        # This ensures we don't try to place a ton of lat and lon values on our axis. 
        # We just want a few to ensure we get an idea of how big the area we traveled is. 
        plt.locator_params(nbins=5)
        
        plt.xlim(xlim[0], xlim[1])
        plt.ylim(ylim[0], ylim[1])
        
        plt.xlabel("Longitude", fontsize=20)
        plt.ylabel("Latitude", fontsize=20)
    
        # This will plot the TIF file that contains the google maps satelite image
        # that is georeferenced to our coordinates. 
        tiff = rasterio.open(tif_file)
        plt.title(f"{title} Path Coordinates", fontsize=22)
        rasterio.plot.show(tiff, with_bounds=True, adjust=False)
        
    
    dels = []
    for ii in range(0, len(measuredData['longitude'])):
        
        h2 = measuredData['distancebetween'].values[ii] 
        
        index = measuredData['index'].values[ii]
        

        
        targetLat    = wayLat[index]
        targetLon    = wayLon[index]
        
        if index-1 < 0:
            index = index + len(wayLat)
        
        oldTargetLat    = wayLat[index-1]
        oldTargetLon    = wayLon[index-1]
        
        a = headingBetween(targetLat,                           \
                            targetLon,                           \
                            measured_lat_data.values[ii], \
                            measured_lon_data.values[ii])
            
        b = headingBetween(targetLat, targetLon, oldTargetLat, oldTargetLon)
        
        beta = b-a
        
        # if beta < 0:
        #     beta = beta+360
        # elif beta >= 360:
        #     beta = beta-360
        
        dels.append(h2*sin(radians(beta)))
        
    if plot: 
        # Create the histogram:
        fig, ax2 = plt.subplots()
        ax2.clear()
        plt.hist(dels, bins=30, color='skyblue', edgecolor='black')
        plt.xlabel(r'$\delta$ (m)', fontsize=20)
        plt.ylabel('Frequency', fontsize=20)
        plt.title(f"{title} Path Deviation", fontsize=22)
        ax2.set_xlim(-2.5, 3.5)
        plt.show()
        
        
            
        # print("RMS IS: ", RMS)
        # MAX = max(dels)s
        # MIN = min(dels)
        # print("MAX: ", MAX)
        # print("MIN: ", MIN)
    
    return dels
        
        
    
    

    
    
    
    