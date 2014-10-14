/** @file: fibo.cpp
 *  @author: Matthew Daws
 *
 *  C++ fibonnaci benchmark, using gmplib.
 *  Contains the C code from Bartosz, by way of comparison; that part is
 *  Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
 *  Licensed under the MIT license.
 *
 * Compile with g++ -std=c++11 -O3 -march=native -mtune=native fibo.cpp -o fibo.exe -lgmpxx -lgmp
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmp.h> // include after stdio.h

void fibo(const int n, bool print) {
	const int bits = 1024*1024; // preallocating avoids reallocing

	mpz_t old;
	mpz_init2(old, bits);
	mpz_set_ui(old, 0UL);

	mpz_t _new;
	mpz_init2(_new, bits);
	mpz_set_ui(_new, 1UL);

	mpz_t sum;
	mpz_init2(sum, bits);

	for (int i = 0; i < (n - 2); ++i) {
		mpz_add(sum, _new, old);
		mpz_swap(old, _new);
		mpz_swap(_new, sum);
	}

	if ( print ) {
		const int base = 10;
		mpz_out_str(stdout, base, _new);
		printf("\n");
	}

	mpz_clear(old);
	mpz_clear(_new);
	mpz_clear(sum);
}

#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <sstream>
#include <gmpxx.h>

using bigint = mpz_class;

void fibo_cpp(const int n, bool print)
{
	bigint a = 0;
	bigint b = 1;
	
	for (int i=0; i<n-2; ++i)
	{
		a += b;
		std:swap(a,b);
	}

	if ( print ) {
		cout << b << endl;
	}
}

#include "timer.hpp"

int main(int argc, char** argv)
{
	if ( argc > 1 )
	{
		int num;
		std::stringstream(std::string(argv[1])) >> num;
		fibo_cpp(num,true);
		return 0;
	}

	cout << "C version:" << endl;
	cout << 1000 << " : " << timeit(100000,[](){ fibo(1000,false); }) << endl;
	cout << 10000 << " : " << timeit(5000,[](){ fibo(10000,false); }) << endl;
	cout << 100000 << " : " << timeit(50,[](){ fibo(100000,false); }) << endl;
	cout << 1000000 << " : " << timeit(1,[](){ fibo(1000000,false); }) << endl;
	cout << 2000000 << " : " << timeit(1,[](){ fibo(2000000,false); }) << endl;

	cout << endl << "C++ version:" << endl;
	cout << 1000 << " : " << timeit(150000,[](){ fibo_cpp(1000,false); }) << endl;
	cout << 10000 << " : " << timeit(7000,[](){ fibo_cpp(10000,false); }) << endl;
	cout << 100000 << " : " << timeit(80,[](){ fibo_cpp(100000,false); }) << endl;
	cout << 1000000 << " : " << timeit(1,[](){ fibo_cpp(1000000,false); }) << endl;
	cout << 2000000 << " : " << timeit(1,[](){ fibo_cpp(2000000,false); }) << endl;

	return 0;
}
