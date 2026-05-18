# -*- coding: utf-8 -*-
"""
Created on Sun Mar 30 14:30:49 2025

@author: Wesley
"""

from plot_path import plotPath
import os
from IPython import get_ipython
import matplotlib.pyplot as plt
from math import sqrt

get_ipython().run_line_magic('matplotlib', 'inline')

def getRMS(deltas): 
    
   delSum = 0
   for x in deltas:
       delSum += x**2 

   square_average = delSum / len(deltas)
   RMS = sqrt(square_average)
   
   return RMS

if __name__ == '__main__':
    
    plot = 1
    
    s_dels = []
    paths = [f for f in os.listdir('./straight2') if f.endswith(".csv") and not f.startswith(('path', 'GPSDATA'))]
    rms_values = []
    xlim = [-82.0072, -82.0050]
    ylim = [33.4924 , 33.49285]
    
    curDir = './straight2'
    for (number, f) in enumerate(paths):
        dels = plotPath(os.path.join(curDir, f), os.path.join(curDir, 'path.csv'), xlim, ylim, f"Line", plot)
        s_dels = s_dels + dels
        
    plt.rcParams['font.family'] = 'monospace'
    plt.rcParams['font.monospace'] = ['Consolas']
    
    plt.hist(s_dels, bins=30, color='blue', edgecolor='black')
    plt.title("Line Path Deviation",fontsize=22)
    
    plt.xlabel(r'$\delta$ (m)', fontsize=20)
    plt.ylabel('Frequency', fontsize=20)

    #plt.xlim(-2.5, 3.5) 
    
    plt.tight_layout()  # Adjust layout for better spacing
    plt.show()
    
    s_rms = getRMS(s_dels)

    
    print("RMS: ", s_rms)
