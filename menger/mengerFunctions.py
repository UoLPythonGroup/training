# Menger Sponge creation and plotting script - in_sponge method adapted from http://www.johndcook.com/blog/2011/08/30/slice-a-menger-sponge/

# Creates list off numpy arrays containing 3D data of whether uniform array position in unit cube is in (1) or out (0) of sponge

import math
import numpy as np
from mayavi import mlab

def outside_unit_cube(x,y,z):
    """ safety check that we are in unit cube """
    if x < 0 or y < 0 or z < 0:
        return 1
    if x > 1 or y > 1 or z > 1:
        return 1
    return 0

def in_sponge( x,y,z, level ):
    """Determine whether a point lies inside the Menger sponge
    after the number of iterations given by 'level.' """
    if outside_unit_cube(x,y,z):
        return 0
    #if x == 1 or y == 1 or z == 1:
    #    return 1
    for i in range(level):
        x *= 3
        y *= 3
        z *= 3
 
        # A point is removed if two of its coordinates
        # lie in middle thirds.
        count = 0
        if int(math.floor(x)) % 3 == 1:
            count += 1
        if int(math.floor(y)) % 3 == 1:
            count += 1
        if int(math.floor(z)) % 3 == 1:
            count += 1
        if count >= 2:
            return 0
 
    return 1


# include an exterior square for contour surface aswell

def nestedMenger(N,h,Levels):
    """ use nested triple loop to do check. Offset x,y,z so theres an outer layer of zeros """
    menger = []
    for level in Levels:
        sponge = np.zeros((N,N,N))
        for i in xrange(N):
            for j in xrange(N):
                for k in xrange(N):
                    sponge[i,j,k]=in_sponge( (i-1)*h,(j-1)*h,(k-1)*h, level )
        menger.append(sponge)

    return menger


def iterMenger(N,h,Levels):
    """ use numpy array iterator to do loop with index flag to get position in array """
    menger = []
    for level in Levels:
        sponge = np.zeros((N,N,N))
        it = np.nditer(sponge, op_flags=['readwrite'], flags=['multi_index'])
        while not it.finished:
            it[0] = in_sponge( (it.multi_index[0]-1)*h,(it.multi_index[1]-1)*h,(it.multi_index[2]-1)*h, level )
            it.iternext()
        menger.append(sponge)

    return menger 


def vectorMenger(xyz,Levels):
    """ vectorised method using array of positions in cube - not working correctly??? """
    menger = []
    for level in Levels:
        sponge = np.zeros((N,N,N))
        sponge[:,:,:] = vectorIn_sponge(xyz[:,:,:,0],xyz[:,:,:,1],xyz[:,:,:,2],level)
        menger.append(sponge)

    return menger


vectorIn_sponge = np.vectorize(in_sponge)
outside_unit_cube = np.vectorize(outside_unit_cube)

@mlab.show    
def plotMenger(menger, Levels):
    """ plot using mayavi VTK wrappers """
    levelPlot=0
    for sponge in menger:
        mlab.figure(figure='Menger Sponge: Level '+str(Levels[levelPlot]),fgcolor=(0, 0, 0), bgcolor=(0.9, 1, 1), size = (600, 500))
        sf = mlab.pipeline.scalar_field(sponge.T, transpose_input_array=False)
        #mlab.pipeline.iso_surface(sf, contours=[1.0], opacity=1.0, colormap='copper')
        contour = mlab.pipeline.contour(sf)
        surf = mlab.pipeline.surface(contour, colormap='hsv')
        # R G B values + transparency array (0-255 for each value, standard LUT size is 256)
        colour = [[255,0,0,255]]*256
        surf.module_manager.scalar_lut_manager.lut.table = colour
        mlab.show(surf)
        levelPlot+=1


