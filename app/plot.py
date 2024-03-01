import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.animation import FuncAnimation
import matplotlib.ticker as ticker

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--input", type=str, required=True, help="Input file")
args = ap.parse_args()

j = 0
value = 0

def init():
    #ax.set_xlim(left=2, right=None)
    #ax.set_ylim(bottom=None, top=7)
    #ax.set_xlim(0, 1000)
    #ax.set_ylim(0, 300000)
    #ln.set_data([], [])
    return ln,

def update(i, f, xdata, ydata, ln):
    global j
    global value
    pullData = f.read()
    dataArray = pullData.split('\n')
    for eachLine in dataArray: # and eachLine.startswith("IRQ:"):
        if len(eachLine) < 1:
            continue
        g = eachLine.split()
        if len(g) < 1:
            continue
        h = g[3]
        if h.isdigit() == False:
             continue
        j += 1
        #value = (2*value + int(h)) / (2*2)
        #value = int(h)
        #print(int(h))
        xdata.append(j)
        ydata[0].append(int(h))
    
    ln.set_data(xdata, ydata[0])

                
    #xdata.append(frame)
    #ydata.append(np.sin(frame))
    #ln.set_data(xdata, ydata[0])
    #p = plt.fill_between(xdata, ydata1, ydata2, facecolor = 'C1', alpha = 0.2)
    return ln,




fig, ax = plt.subplots()
ax.set(xlim=(None, 1e4), ylim=(None, 5e6), title='ADC', xlabel='Time (s)', ylabel='Voltage (µV)')
#ax.set(xlim=(None, 1e5), ylim=(None, 1e6), title='ADC', xlabel='Time (s)', ylabel='Ampere (pA)')
#ax.xaxis.set_major_locator(AutoMinorLocator(10))
#ax.xaxis.set_minor_locator(AutoMinorLocator(1))
#ax.xaxis.set_major_locator(ticker.AutoLocator())
#ax.xaxis.set_minor_locator(ticker.AutoMinorLocator())
#ax.xaxis.set_major_locator(ticker.MultipleLocator(100))
#ax.xaxis.set_minor_locator(ticker.MultipleLocator(10))
ax.xaxis.set_major_locator(ticker.MaxNLocator(10))
ax.xaxis.set_minor_locator(ticker.MaxNLocator(100))
ax.ticklabel_format(axis='both', style='plain')
#plt.xticks(np.arange(0, 1e4, 5))
ax.grid(True)
xdata = []
ydata = [[],[],[]] # Avg, Min, Max
ln, = plt.plot([], [], lw=2)

# https://stackoverflow.com/questions/23918120/weird-characters-while-reading-file-content
f = open(args.input, 'r', encoding='utf-8-sig')
#f = open("YAT-Log-20230626-171849.log","r")
#f = open("YAT-Log-20230622-130141.log","r")
#plt.xticks(rotation=45, ha='right')
#plt.subplots_adjust(bottom=0.30)
#plt.title('TMP102 Temperature over Time')
#plt.ylabel('Temperature (deg C)')
#plt.xlim(left=0)

ani = FuncAnimation(fig, update, init_func=init, blit=True, fargs=(f, xdata, ydata, ln), interval=100)
plt.show()