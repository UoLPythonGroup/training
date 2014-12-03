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
#                           0 1     : LONG time, 1.6GB (0.9GB for most of time)
#                           1 1     : 1330 sec, 1.6GB (0.9GB for most of time)
#             Of note is that 2 processes are running constantly at about 70-80%.
#                           4 1     : ???, 1.6GB (0.9GB for most of time)
#             Of note is that now 4 processes are running at about 30%, and one process at 120% (so >1 thread)
#
# Linux, i5-2400 (4 cores/threads) Python 2.7.8
# python memory.py 20000000 0 0     : 3.6 sec, 1GB
#                           1 0     : 3.7 sec, 1GB
#                           2 0     : 3.7 sec, 1.3GB
#                           3 0     : 3.8 sec, 1.8GB
#                           4 0     : 4.1 sec, 2.3GB
#                           0 1     : LONG TIME, 1.6GB
#                           1 1     : >10 mins, 1.6GB
#             Of note is that 2 threads are running constantly, one at about 40-50% and
#               one at about 60-70%.  Also of note is that the linux kernel isn't running
#               any of the cores at maximum frequency-- perhaps too much time is being spent
#               waiting for the OS to process messages?
#
# Linux, i5-2400 (4 cores/threads) Python 3.4.1
# python memory.py 20000000 0 0     : 4.1 sec, 1.3GB
#                           1 0     : 4.1 sec, 1.3GB
#                           2 0     : 4.2 sec, 2GB
#                           3 0     : 4.2 sec, 2.4GB
#                           4 0     : 4.6 sec, 3.1GB
#                           0 1     : 4700 sec, 0.9GB (peaks at more)
#                           1 1     : LONG time, 0.9GB (peaks at more)
#             Same processor core behaviour as observed above
#
# With for "x in arr:" loop instead of while loop: Using a list, the results are
# quicker, (1.3 sec vs 4.1 sec).  Similarly, using an Array, the calculation is maybe
# twice as fast ("0 1" gives 2260 secs on Python3 on Linux, 930 secs for Python2).


# Python 2.X friendly
from __future__ import print_function

import time
import multiprocessing as mp
from sys import argv

def consume(arr, res, lock):
	"Just a silly function to do 'work' on a big array"
	total = 0
	#for x in arr: total += x
	# Should maybe use (x)range...
	i = 0
	while i < len(arr):
		total += arr[i]
		i += 1
	# Critical section to add total in
	with lock:
		res.value += total


if __name__ == "__main__":
	if len(argv) < 3:
		print("Call with parameters {len}, {num sub-processes}, {0/1 for list / Array}.")
		print("Set {num sub-processes} to 0 to run as single process.")
		exit()
	N = int(argv[1])
	cores = int(argv[2])
	data = list(range(N))

	manager = mp.Manager()
	lock = mp.Lock()
	res = manager.Value('i',0)

	if argv[3] == "0":
		arr = data
		print("N=",N,"processes=",cores,"with list.")
	else:
		arr = manager.Array('i', data)
		print("N=",N,"processes=",cores,"with mp.Array.")

	c1 = time.time()
	if cores == 0:
		#consume(list(range(N)),res,lock)
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

