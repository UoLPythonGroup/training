#cython: boundscheck=False, wraparound=False
import numpy as np
cimport numpy as np

def rotT(np.ndarray[np.float64_t, ndim=4] T,
         np.ndarray[np.float64_t, ndim=2] g):
    cdef np.ndarray[np.float64_t, ndim=4] Tprime
    cdef Py_ssize_t i, j, k, l, ii, jj, kk, ll
    cdef np.float64_t gg

    Tprime = np.zeros((3,3,3,3), dtype=T.dtype)
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

