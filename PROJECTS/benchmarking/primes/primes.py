# Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
# This program is free software under the MIT license.

import sys

def index(num):
	return int((num / 2) - 1)

def primes(maxnum):
	p0 = 2
	p1 = 3
	step = 2

	# calculate
	isprime = [True] * int((maxnum - 1) / 2)
	num = p1
	while num <= maxnum:
		rem = 3 * num
		while rem <= maxnum:
			isprime[index(rem)] = False
			rem += 2 * num
		num += step
		while num <= maxnum and not isprime[index(num)]:
			num += step

	"""
	# print
	print(p0, end=" ")
	for p in range(p1, maxnum, step):
		if isprime[index(p)]:
			print(p, end=" ")
	print()
	"""

if __name__ == "__main__":
	maxnum = int(sys.argv[1])
	primes(maxnum)