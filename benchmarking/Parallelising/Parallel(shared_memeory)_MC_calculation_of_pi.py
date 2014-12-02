from multiprocessing import Process,Queue,Manager,Value,Lock
from random import random
from sys import argv

def get_pi(N,pi,lock):
  """Monte Carlo method of calculating pi the first argument is the number of samples to use, the second one in this case it for the return value."""
  n=0
  while n<N:
    x=random()
    y=random()
    n+=1
    if x*x+y*y<=1.0:
      with lock: #this bit is very important, it makes sure that two threads don't access pi at the same time.
        pi.value+=1

if __name__ == '__main__':#needs to be used because on widows all processes start from the beginning due to the lack of the fork() function. This ensures on the master process runs this bit
  N=int(argv[1])#Number of samples
  manager = Manager()
  pi=manager.Value('i',0)
  lock=Lock()
  cores=int(argv[2])#Number of cores to use
  p=[]#Empty lists for processes
  for i in range(0,cores,1):#start all processes
    p.append(Process(target=get_pi,args=(N/cores,pi,lock)))#create the process
    p[i].start()#start the process
  for i in range(0,cores,1):
    p[i].join()#wait until all processes get to here
  print 4.0*float(pi.value)/float(N)#print the average
