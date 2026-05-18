# -*- coding: utf-8 -*-
"""
Created on Fri Apr 11 18:04:05 2025

@author: Wesley
"""


if __name__ == "__main__":
    from IPython import get_ipython
    import matplotlib.pyplot as plt
    from math import sqrt
    import csv 
    
    get_ipython().run_line_magic('matplotlib', 'inline')

    def getRMS(deltas): 
        
       delSum = 0
       for x in deltas:
           delSum += x**2 

       square_average = delSum / len(deltas)
       RMS = sqrt(square_average)
       
       return RMS

    
    square_dels = []
    circle_dels = []
    trap_dels   = []
    elipse_dels = []

    with open('ccw_data.csv', 'r') as file_ccw, open('cw_data.csv', 'r') as file_cw:
        csv_reader_ccw = csv.reader(file_ccw)
        csv_reader_cw  = csv.reader(file_cw)
    
        # Each line is a list of strings, so convert to floats
        square_dels = list(map(float, next(csv_reader_ccw))) + list(map(float, next(csv_reader_cw)))
        circle_dels = list(map(float, next(csv_reader_ccw))) + list(map(float, next(csv_reader_cw)))
        trap_dels   = list(map(float, next(csv_reader_ccw))) + list(map(float, next(csv_reader_cw)))
        elipse_dels = list(map(float, next(csv_reader_ccw))) + list(map(float, next(csv_reader_cw)))
    
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
    
    axes.flat[1].set_xlim(-2.5, 5.5)
    
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
    axes2.set_xlim(-2.5, 5.5)
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
    
