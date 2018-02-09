# Menger Sponge creation and plotting script - in_sponge method adapted from http://www.johndcook.com/blog/2011/08/30/slice-a-menger-sponge/

# Creates list off numpy arrays containing 3D data of whether uniform array position in unit cube is in (1) or out (0) of sponge

import math
import numpy as np
from mayavi import mlab
import mengerFunctions as mF

# pick range of menger sponge levels to create
Levels = [0,1,2,3]

# set detail level of array plus array spacing, larger N will be required for higher level cubes
N=102

# leave an exterior 0 to get faces plotted correctly - without this contour would only plot interior of sponge
h=1.0/(N-3)

# array of x,y,z values for vector menger function - UNCOMMENT to use with vectorised method
#xyz = np.array([[[(i*h,j*h,k*h) for k in xrange(-1,N-1)] for j in xrange(-1,N-1)] for i in xrange(-1,N-1)])

# ipython magic functions to do timing testing if so desired
#%timeit menger1 = mF.nestedMenger(N,h,Levels)
#%timeit menger2 = mF.iterMenger(N,h,Levels)
#%timeit menger3 = mF.vectorMenger(xyz,Levels)

# this one was actually quickest on my machine
menger = mF.nestedMenger(N,h,Levels)

# plot generated sponges (each sponge will open in its own window and can be individually manipulated)
mF.plotMenger(menger, Levels)

