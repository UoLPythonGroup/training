Most of the example is in the ipython
notebook. Launch with:
$ ipython notebook

To build the cpython extension do:
$ python ./setup.py build_ext --inplace

To run the benchmarks:
$ ./rot_tensor.py

Further reading:

http://docs.cython.org

http://nbviewer.ipython.org/github/ipython/ipython/blob/3607712653c66d63e0d7f13f073bde8c0f209ba8/docs/examples/notebooks/cython_extension.ipynb

http://docs.cython.org/src/userguide/numpy_tutorial.html

https://python.g-node.org/python-summerschool-2011/_media/materials/cython/cython-slides.pdf

NB: The python setup on my laptop is a 
bit broken. You should not need to do 
include_dirs = ... in setup.py or
in the notebook.



