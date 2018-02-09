from random import random
from sys import argv

def get_pi(N):
  """Monte Carlo method of calculating pi the argument is the number of samples to use."""
  pi=0
  n=0
  while n<N:
    x=random()
    y=random()
    n+=1
    if x*x+y*y<=1.0:
      pi+=1
  return 4.0*float(pi)/float(n)

N=int(argv[1])
print get_pi(N)
