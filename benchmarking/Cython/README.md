Most of the example is in the ipython
notebook. Launch with:
$ ipython notebook

To build the cpython extension do:
$ python ./setup.py build_ext --inplace

To run the benchmarks:
$ ./rot_tensor.py

NB: The python setup on my laptop is a 
bit broken. You should not need to do 
include_dirs = ... in setup.py or
in the notebook.



