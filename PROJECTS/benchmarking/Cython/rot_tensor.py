#!/usr/bin/env python

import numpy as np

def rotT(T, g):
    Tprime = np.zeros((3,3,3,3))
    for i in range(3):
        for j in range(3):
            for k in range(3):
                for l in range(3):
                    for ii in range(3):
                        for jj in range(3):
                            for kk in range(3):
                                for ll in range(3):
                                    gg = g[ii,i]*g[jj,j]*g[kk,k]*g[ll,l]
                                    Tprime[i,j,k,l] = Tprime[i,j,k,l] + \
                                         gg*T[ii,jj,kk,ll]
    return Tprime


if __name__ == "__main__":
    # If this gets run as a script, execution starts here...
    # ... just time the options and check results are the same
    import timeit 
    import numpy.testing as npt
    

    # An example rank-4 tensor
    T = np.array([[[[  4.66533067e+01,  5.84985000e-02, -5.37671310e-01],
                    [  5.84985000e-02,  1.56722231e+01,  2.32831900e-02],
                    [ -5.37671310e-01,  2.32831900e-02,  1.33399259e+01]],
                   [[  4.60051700e-02,  1.54658176e+01,  2.19568200e-02],
                    [  1.54658176e+01, -5.18223500e-02, -1.52814920e-01],
                    [  2.19568200e-02, -1.52814920e-01, -2.43874100e-02]],
                   [[ -5.35577630e-01,  1.95558600e-02,  1.31108757e+01],
                    [  1.95558600e-02, -1.51342210e-01, -6.67615000e-03],
                    [  1.31108757e+01, -6.67615000e-03,  6.90486240e-01]]],
                  [[[  4.60051700e-02,  1.54658176e+01,  2.19568200e-02],
                    [  1.54658176e+01, -5.18223500e-02, -1.52814920e-01],
                    [  2.19568200e-02, -1.52814920e-01, -2.43874100e-02]],
                   [[  1.57414726e+01, -3.86167500e-02, -1.55971950e-01],
                    [ -3.86167500e-02,  4.65601977e+01, -3.57741000e-02],
                    [ -1.55971950e-01, -3.57741000e-02,  1.34215636e+01]],
                   [[  2.58256300e-02, -1.49072770e-01, -7.38843000e-03],
                    [ -1.49072770e-01, -3.63410500e-02,  1.32039847e+01],
                    [ -7.38843000e-03,  1.32039847e+01,  1.38172700e-02]]],
                  [[[ -5.35577630e-01,  1.95558600e-02,  1.31108757e+01],
                    [  1.95558600e-02, -1.51342210e-01, -6.67615000e-03],
                    [  1.31108757e+01, -6.67615000e-03,  6.90486240e-01]],
                   [[  2.58256300e-02, -1.49072770e-01, -7.38843000e-03],
                    [ -1.49072770e-01, -3.63410500e-02,  1.32039847e+01],
                    [ -7.38843000e-03,  1.32039847e+01,  1.38172700e-02]],
                   [[  1.33639532e+01, -1.26331100e-02,  6.84650400e-01],
                    [ -1.26331100e-02,  1.34222177e+01,  1.67851800e-02],
                    [  6.84650400e-01,  1.67851800e-02,  4.89151396e+01]]]])

    # An example rotation matrix
    g = np.array([[ 0.79389393,  0.54184237,  0.27593346],
                  [-0.59925749,  0.62028664,  0.50609776],
                  [ 0.10306737, -0.56714313,  0.8171449 ]])

    print ""
    print "+==============================================+"
    print "|                                              |"
    print "| Tensor rotation benchmarks                   |"
    print "|                                              |"
    print "+==============================================+"
    print ""

    print "  Naive python (looping) implementation"
    print "        %.4f msec/pass" % (1000 * timeit.timeit(
                       "rotT(T, g)", "from __main__ import g, T, rotT", number=500)/500)


    print "  Translated to C (cython) implementation"
    try:
        from rot_tensor_cython import rotT as rotT_cpy
        npt.assert_almost_equal(rotT(T, g), rotT_cpy(T, g))
        print "        %.4f msec/pass" % (1000 * timeit.timeit(
                       "rotT_cpy(T, g)", "from __main__ import g, T, rotT_cpy", number=500)/500)

    except ImportError:
        print "        skipped - module could not be imported"
       
