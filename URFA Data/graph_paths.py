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
    square_dels = []
    circle_dels = []
    trap_dels   = []
    elipse_dels = []
    square_paths = [f for f in os.listdir('./Data/square') if f.endswith(".xlsx")]
    elipse_paths = [f for f in os.listdir('./Data/Elipse') if f.endswith(".xlsx")]
    circle_paths = [f for f in os.listdir('./Data/Circle') if f.endswith(".xlsx")]
    trap_paths   = [f for f in os.listdir('./Data/Trap') if f.endswith(".xlsx")]
    
    rms_values = []
    
    xlim = [-82.0057, -82.0050]
    ylim = [33.4923 , 33.4928]
    
    curDir = './Data/square'
    for (number, f) in enumerate(square_paths):
        dels = plotPath(os.path.join(curDir, f), os.path.join(curDir, 'path.csv'), xlim, ylim, f"Square {number+1}", plot)
        square_dels = square_dels + dels
        
    curDir = './Data/Circle'
    for (number, f) in enumerate(circle_paths):
        dels = plotPath(os.path.join(curDir, f), os.path.join(curDir, 'path.csv'), xlim, ylim, f"Circle {number+1}", plot)
        circle_dels = circle_dels + dels
        
    xlim = [-82.0058, -82.0050]
    ylim = [33.4924 , 33.49285]
    
    curDir = './Data/Elipse'
    for (number, f) in enumerate(elipse_paths): 
        dels = plotPath(os.path.join(curDir, f), os.path.join(curDir, 'path.csv'), xlim, ylim, f"Elipse {number+1}", plot)
        elipse_dels = elipse_dels + dels
        
    curDir = './Data/Trap'
    for (number, f) in enumerate(trap_paths):
        dels = plotPath(os.path.join(curDir, f), os.path.join(curDir, 'path.csv'), xlim, ylim, f"Trapezoid {number+1}", plot)
        trap_dels = trap_dels + dels
    
    fig, axes = plt.subplots(2, 2, figsize=(10, 8))  # 2x2 grid of subplots
    plt.rcParams['font.family'] = 'monospace'
    plt.rcParams['font.monospace'] = ['Consolas']
    
    axes[0, 0].hist(square_dels, bins=30, color='blue', edgecolor='black')
    axes[0, 0].set_title("Square Path Deviation",fontsize=22)
    
    axes[0, 1].hist(trap_dels, bins=30, color='red', edgecolor='black')
    axes[0, 1].set_title("Trapezoid Path Deviation",fontsize=22)
    
    axes[1, 0].hist(circle_dels, bins=30, color='green', edgecolor='black')
    axes[1, 0].set_title("Circle Path Deviation",fontsize=22)
    
    axes[1, 1].hist(elipse_dels, bins=30, color='purple', edgecolor='black')
    axes[1, 1].set_title("Ellipse Path Deviation", fontsize=22)
    
    for ax in axes.flat:
        ax.set_xlabel(r'$\delta$ (m)', fontsize=20)
        ax.set_ylabel('Frequency', fontsize=20)

    for ax in axes.flat:
        ax.set_xlim(-2.5, 3.5)
    
    plt.tight_layout()  # Adjust layout for better spacing
    plt.show()
    
    square_rms = getRMS(square_dels)
    circle_rms = getRMS(circle_dels)
    trap_rms   = getRMS(trap_dels)
    elipse_rms = getRMS(elipse_dels)
    
    print("Square: ", square_rms, end= " ")
    print("Trap:   ", trap_rms)
    print("Circle: ", circle_rms, end= " ")
    print("Elipse: ", elipse_rms)
    
    rms_sum = square_rms + circle_rms + trap_rms + elipse_rms
    RMS     = rms_sum/4
    print("Average: ", RMS)
    
    fig2, axes2 = plt.subplots()
    plt.rcParams['font.family'] = 'monospace'
    plt.rcParams['font.monospace'] = ['Consolas']
    axes2.hist(square_dels, bins=30, color='blue', edgecolor='black')
    axes2.set_title("Square Path Deviation", fontsize=22)
    axes2.set_xlabel(r'$\delta$ (m)', fontsize=20)
    axes2.set_ylabel('Frequency', fontsize=20)
    axes2.set_xlim(-2.5, 3.5)
    plt.show()
    
    fig2, axes2 = plt.subplots()
    plt.rcParams['font.family'] = 'monospace'
    plt.rcParams['font.monospace'] = ['Consolas']
    axes2.hist(trap_dels, bins=30, color='red', edgecolor='black')
    axes2.set_title("Trapezoid Path Deviation", fontsize=22)
    axes2.set_xlabel(r'$\delta$ (m)', fontsize=20)
    axes2.set_ylabel('Frequency', fontsize=20)
    axes2.set_xlim(-2.5, 3.5)
    plt.show()
    
    
    fig2, axes2 = plt.subplots()
    plt.rcParams['font.family'] = 'monospace'
    plt.rcParams['font.monospace'] = ['Consolas']
    axes2.hist(circle_dels, bins=30, color='green', edgecolor='black')
    axes2.set_title("Circle Path Deviation",fontsize=22)
    axes2.set_xlabel(r'$\delta$ (m)', fontsize=20)
    axes2.set_ylabel('Frequency', fontsize=20)
    axes2.set_xlim(-2.5, 3.5)
    plt.show()
    
    fig2, axes2 = plt.subplots()
    plt.rcParams['font.family'] = 'monospace'
    plt.rcParams['font.monospace'] = ['Consolas']
    axes2.hist(elipse_dels, bins=30, color='purple', edgecolor='black')
    axes2.set_title("Ellipse Path Deviation",fontsize=22)
    axes2.set_xlabel(r'$\delta$ (m)', fontsize=20)
    axes2.set_ylabel('Frequency', fontsize=20)
    axes2.set_xlim(-2.5, 3.5)
    plt.show()
    
    import csv 
    file_path = 'ccw_data.csv'
    with open(file_path, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(square_dels)
        csv_writer.writerow(circle_dels)
        csv_writer.writerow(trap_dels)
        csv_writer.writerow(elipse_dels)
    