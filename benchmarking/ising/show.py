import matplotlib.pyplot as plt
import numpy as np


import argparse

parser = argparse.ArgumentParser(description='Show the output of the C++ programme')
parser.add_argument('filename', metavar='file', type=str, nargs='+',
                   help='filename to display')

args = parser.parse_args()


with open(args.filename[0]) as f:
    L = int( f.readline() )
    lattice = np.zeros((L,L))
    for row, line in enumerate(f):
        nums = line.split(",")
        for x, val in enumerate(nums):
            lattice[x,row] = int( val )

plt.imshow(lattice,interpolation="none", cmap="bone")
plt.show()
