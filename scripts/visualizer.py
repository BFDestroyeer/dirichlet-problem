import argparse
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import struct
from mpl_toolkits.mplot3d import Axes3D


def main(args):
    file = open(args.path, 'rb')
    size = struct.unpack('Q', file.read(8))[0]
    array = np.zeros((size, size))
    for i in range(size):
        for j in range(size):
            array[i][j] = struct.unpack('d', file.read(8))[0]

    x, y = np.meshgrid(np.linspace(0, 1, size), np.linspace(0, 1, size))
    z = array

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(x, y, z, rcount=20, ccount=20)
    plt.savefig('plot.png')


argparser = argparse.ArgumentParser()
argparser.add_argument('path', type=str, action='store')

args = argparser.parse_args()

main(args)
