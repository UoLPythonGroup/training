# Ising model #

A very simple Ising model (at zero temperature).

Currently some IPython notebooks setting up some simple classes.  I wonder if there is a _better_ way to do this: my code is very like a C programmer in Python...

Also some very basic C++ code, which is much much quicker.  Todo: Implement the ideas to speed up the stochastic simulation, and see if it really makes much difference in the C++ implementation.


## File details ##

  - Intro; 1D Ising.ipynb : Introduction, and 1D version
  - 2D Ising.ipynb : 2D version and some graphs showing how, quite quickly, most of the action happens in a small subset of the vertex sites.
  - Faster 2D Ising.ipynb : Some musings on how to make a quicker algorithm.
  - ising2d.cpp : Basic, all in one, C++ file (C++11)
  - show.py : Simple Python file to display the text output files which the C++ programme produces.  Uses `argparse` module to get command line options.