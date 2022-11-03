from cProfile import label
from turtle import color
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

def parseApPos(filename):
    df = pd.read_csv(filename, index_col=False)
    df = df[['bssid','x','z']][df['obs'] > 5]
    df = df.reset_index()

    x = df.x.to_list()
    z = df.z.to_list()
    bssid = df.bssid.to_list()
    return x, z ,bssid

def parseTraj(filename):
    cols = ['timestamp', 'x','y','z','qx','qy','qz','qw']
    df = pd.read_csv('KeyFrameTrajectory.txt', sep=" ", index_col=False, names = cols)
    df = df[['x','z']].reset_index()

    return df.x.to_list(), df.z.to_list()

def plot(x, z, bssid, traj_x, traj_z):
    fig=plt.figure()
    ax=fig.gca()
    ax.set_xticks(np.arange(-10, 40, 5))
    ax.set_yticks(np.arange(-20, 30., 5))
    ax.set_xlabel('x (m)')
    ax.set_ylabel('z (m)')
    ax.set_title('Ap Position')
    plt.scatter(x, z, color='r', label="Ap")
    plt.plot(traj_x, traj_z, '.r-', color = 'b', label = "Trajectory") 
    plt.grid()
    plt.legend(loc='upper right')
    plt.show()
    

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print('python apPlot.py <apfile> <traj file>')
        sys.exit()

    apfile = sys.argv[1]
    trajfile = sys.argv[2]

    x, z, bssid = parseApPos(apfile)
    traj_x , traj_z = parseTraj(trajfile)
    plot(x,z,bssid, traj_x, traj_z)