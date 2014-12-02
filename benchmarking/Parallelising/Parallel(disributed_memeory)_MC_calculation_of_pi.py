from multiprocessing import Process,Queue
from random import random
from sys import argv

def get_pi(N,q):
  """Monte Carlo method of calculating pi the first argument is the number of samples to use, the second one in this case it for the return value."""
  pi=0
  n=0
  while n<N:
    x=random()
    y=random()
    n+=1
    if x*x+y*y<=1.0:
      pi+=1
  q.put(4.0*float(pi)/float(n))

if __name__ == '__main__':#needs to be used because on widows all processes start from the beginning due to the lack of the fork() function. This ensures on the master process runs this bit
  N=int(argv[1])#Number of samples
  cores=int(argv[2])#Number of cores to use
  q=[]#Empty lists for return values and processes
  p=[]
  for i in range(0,cores,1):#start all processes
    q.append(Queue())#create a queue to store the return value in
    p.append(Process(target=get_pi,args=(N/cores,q[i])))#create the process
    p[i].start()#start the process
  for i in range(0,cores,1):
    p[i].join()#wait until all processes get to here
  pi=q[0].get()#get all the return values
  for i in range(1,cores,1):
    pi+=q[i].get()
  print pi/float(cores)#print the average
