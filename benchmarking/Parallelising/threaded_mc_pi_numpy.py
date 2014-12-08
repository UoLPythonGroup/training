from __future__ import print_function
import numpy as np
import threading
import sys
import time

def mc_pi_numpy_chunks(samples, maxchunk=25000):
    left = samples
    count = 0
    while left > 0:
        dothistime = min(maxchunk,left)
        x = np.random.rand(dothistime)
        c = x*x
        x = np.random.rand(dothistime)
        c += x*x
        count += np.sum(c<=1.0)
        left -= maxchunk
    return 4.0 * count / samples

class mc_pi_thread(threading.Thread):
    # function to do the run
    def run_mc_pi(self, iters):
        self.result = mc_pi_numpy_chunks(iters, self.maxchunk)
    def __init__(self, iters, maxchunk = 25000):
        # Just delegate to the Thread constructor
        self.maxchunk = maxchunk # Largest chuck to process at once
        threading.Thread.__init__(self, target=mc_pi_thread.run_mc_pi, args=(self,iters))
        threading.Thread.start(self)
    def get(self):
        threading.Thread.join(self)
        return self.result

def mc_pi_numpy_threads(samples, threads, maxchunk = 25000):
    t = []
    for x in range(threads):
        t.append( mc_pi_thread(samples//threads, maxchunk) )
    ourpi = 0
    for x in range(threads):
       ourpi += t[x].get()
    return ourpi/threads

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Call with parameters {len}, {num sub-threads}.")
        print("Set {num sub-processes} to 0 to run as single thread.")
        exit()
    N = int(sys.argv[1])
    threads = int(sys.argv[2])

    c1 = time.time()
    if threads == 0:
        pi = mc_pi_numpy_chunks(N)
    else:   
        pi = mc_pi_numpy_threads(N,threads)
    c2 = time.time()
    
    print("Estimate of pi =",pi,"time taken =",c2-c1)
