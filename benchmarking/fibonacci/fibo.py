# Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
# This program is free software under the MIT license.

import sys

def fibo(n):
	old, new = 0, 1
	for i in range(n - 2):
		old, new = new, new + old
	print(new)

if __name__ == "__main__":
	fibo(int(sys.argv[1]))
