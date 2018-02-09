// Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
// This program is free software under the MIT license.

#include <iso646.h> // and, not
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset()

int index(const int num) {
	return (num / 2) - 1;
}

void primes(const int maxnum) {
	const int p0 = 2;
	const int p1 = 3;
	const int step = 2;

	// allocate
	const int len = (maxnum - 1) / 2;
	char* isprime = malloc(len * sizeof(char));
	if (isprime == NULL) {
		printf("isprime == NULL\n");
		exit(EXIT_FAILURE);
	}
	memset(isprime, 1, len); // 1 = true

	// calculate
	int num = p1;
	while (num <= maxnum) {
		int rem = 3 * num;
		while (rem <= maxnum) {
			isprime[index(rem)] = 0;
			rem += 2 * num;
		}
		num += step;
		while ((num <= maxnum) and (not isprime[index(num)])) {
			num += step;
		}
	}

	/*
	// print
	printf("%d ", p0);
	for (int p = p1; p < maxnum; p += step) {
		if (isprime[index(p)] == 1) {
			printf("%d ", p);
		}
	}
	printf("\n");
	*/

	free(isprime);
}

int main(int argc, char** argv) {
	const int maxnum = strtol(argv[1], NULL, 10);
	primes(maxnum);
	exit(EXIT_SUCCESS);
}