from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension("rot_tensor_cython", ["rot_tensor_cython.pyx"],
    include_dirs = ["/Applications/Canopy.app/appdata/canopy-1.4.0.1938.macosx-x86_64/Canopy.app/Contents/lib/python2.7/site-packages/numpy/core/include/"])]
)

