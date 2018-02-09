# With some alternations suggested by Andrew Walker.
# Basically, we shouldn't use a Manager, unless we actually need the facilities provided.
#
# Performs some work on a big array.  Running mutliple sub-processes just repeats the work,
# so shouldn't expect a speed-up (but by the same token, this should be perfectly parallel
# work-- ignoring Cache issues-- so in theory there also shouldn't be slow down).
#
# Results follow.  Memory usage is crudely derived from looking at the GUI task manager.
#
# Windows 8.1, i3-4160 (turbo-boost on... 2 cores, 4 threads), Anaconda Python 3.4.1 64-bit
# python memory.py 20000000 0 0     : 3.8 sec, 0.8GB
#                           1 0     : 6.2 sec, 1.5GB
#                           2 0     : 8.6 sec, 2.3GB
#                           3 0     : 11.7 sec, 3GB
#                           4 0     : 14.8 sec, 3.6GB
#                           0 1     : 4.3 sec, rather little
#                           1 1     : 4.5 sec, rather little
#                           2 1     : 5.7 sec, rather little
#                           3 1     : 7.6 sec, rather little
#                           4 1     : 10.7 sec, rather little
#      Even with a much larger array, the memory usage doesn't change between 0 and 4
#      sub-processes, so memory is being genuinely shared.
#
# Linux, i5-2400 (4 cores/threads) Python 2.7.8
# python memory.py 20000000 0 0     : TODO
#
# Linux, i5-2400 (4 cores/threads) Python 3.4.1
# python memory.py 20000000 0 0     : TODO
#
# With for "x in arr:" loop instead of while loop: Using a list, the results are
# quicker, (1.3 sec vs 4.1 sec).  A similar, but much less pronounced, effect with Array.


# Python 2.X friendly
from __future__ import print_function

import time
import multiprocessing as mp
import sys

def consume(arr, res, lock):
    "Just a silly function to do 'work' on a big array"
    total = 0
    for x in arr: total += x
    # Should maybe use (x)range...
    #i = 0
    #while i < len(arr):
    #    total += arr[i]
    #    i += 1
    # Critical section to add total in
    with lock:
        res.value += total


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Call with parameters {len}, {num sub-processes}, {0/1 for list / Array}.")
        print("Set {num sub-processes} to 0 to run as single process.")
        exit()
    N = int(sys.argv[1])
    cores = int(sys.argv[2])

    manager = mp.Manager()
    lock = mp.Lock()
    # Do want to use the manager here, as it will allow res to be a Python (long) integer;
    # otherwise seems 
    res = manager.Value('i',0)

    if sys.argv[3] == "0":
        # If Python 3 then wrap in list; but don't do this for Python to avoid possible copy
        if sys.version_info.major==2:
            arr = range(N)
        else:
            arr = list( range(N) )
        print("N=",N,"processes=",cores,"with list.")
    else:
        #arr = mp.Array('i', range(N), lock=False)
        # Slower, but uses much less memory
        arr = mp.Array('i', N, lock=False)
        func = xrange if sys.version_info.major==2 else range
        for i in func(N):
            arr[i] = i
        print("N=",N,"processes=",cores,"with mp.Array.")

    c1 = time.time()
    if cores == 0:
        consume(arr,res,lock)
        c2 = time.time()
        print("Time:", c2-c1)
        print(res.value)
        exit()

    p = []
    for i in range(cores):
        p.append( mp.Process(target=consume,args=(arr, res, lock)) )
        p[i].start()
    for i in range(cores):
        p[i].join()

    c2 = time.time()
    print("Time:", c2-c1)
    print(res)
