// Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
// This program is free software under the MIT license.
//
// Timing code added by Matt Daws; partly converted to C++ for this
// g++ -std=c++11 -O3 -march=native -mtune=native primes_time.cpp -o primes_time.exe

#include <iso646.h> // and, not
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset()

int index(const int num) {
	return (num / 2) - 1;
}

void primes(const int maxnum, const bool print) {
	const int p0 = 2;
	const int p1 = 3;
	const int step = 2;

	// allocate
	const int len = (maxnum - 1) / 2;
	char* isprime = (char*)malloc(len * sizeof(char));
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

	if ( print ) {
		printf("%d ", p0);
		for (int p = p1; p < maxnum; p += step) {
			if (isprime[index(p)] == 1) {
				printf("%d ", p);
			}
		}
		printf("\n");
	}

	free(isprime);
}

#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <sstream>
#include "timer.tpp"

int main(int argc, char** argv) {
	if ( argc > 1 )
	{
		int num;
		std::stringstream(std::string(argv[1])) >> num;
		primes(num,true);
		return 0;
	}

	/*cout << "1,000,000 : " << timeit(500,[](){ primes(1000000,false); }) << endl;
	cout << "1,000,000 : " << timeit(500,[](){ primes(1000000,false); }) << endl;
	cout << "1,000,000 : " << timeit(500,[](){ primes(1000000,false); }) << endl;

	cout << "3,000,000 : " << timeit(200,[](){ primes(3000000,false); }) << endl;
	cout << "3,000,000 : " << timeit(200,[](){ primes(3000000,false); }) << endl;
	cout << "3,000,000 : " << timeit(200,[](){ primes(3000000,false); }) << endl;

	cout << "10,000,000 : " << timeit(50,[](){ primes(10000000,false); }) << endl;
	cout << "10,000,000 : " << timeit(50,[](){ primes(10000000,false); }) << endl;
	cout << "10,000,000 : " << timeit(50,[](){ primes(10000000,false); }) << endl;

	cout << "30,000,000 : " << timeit(10,[](){ primes(30000000,false); }) << endl;
	cout << "30,000,000 : " << timeit(10,[](){ primes(30000000,false); }) << endl;
	cout << "30,000,000 : " << timeit(10,[](){ primes(30000000,false); }) << endl;*/

	cout << "100,000,000 : " << timeit(3,[](){ primes(100000000,false); }) << endl;
	cout << "100,000,000 : " << timeit(3,[](){ primes(100000000,false); }) << endl;
	cout << "100,000,000 : " << timeit(3,[](){ primes(100000000,false); }) << endl;

	cout << "300,000,000 : " << timeit(1,[](){ primes(300000000,false); }) << endl;
	cout << "300,000,000 : " << timeit(1,[](){ primes(300000000,false); }) << endl;
	cout << "300,000,000 : " << timeit(1,[](){ primes(300000000,false); }) << endl;

	return 0;
}