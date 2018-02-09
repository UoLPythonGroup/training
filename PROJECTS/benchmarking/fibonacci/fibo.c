// Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
// This program is free software under the MIT license.

// uses https://gmplib.org, compile with -lgmp

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmp.h> // include after stdio.h

void fibo(const int n) {
	const int bits = 1024*1024; // preallocating avoids reallocing

	mpz_t old;
	mpz_init2(old, bits);
	mpz_set_ui(old, 0UL);

	mpz_t new;
	mpz_init2(new, bits);
	mpz_set_ui(new, 1UL);

	mpz_t sum;
	mpz_init2(sum, bits);

	for (int i = 0; i < (n - 2); ++i) {
		mpz_add(sum, new, old);
		mpz_swap(old, new);
		mpz_swap(new, sum);
	}

	const int base = 10;
	mpz_out_str(stdout, base, new);
	printf("\n");

	mpz_clear(old);
	mpz_clear(new);
	mpz_clear(sum);
}

int main(int argc, char** argv) {
	fibo(strtol(argv[1], NULL, 10));
	exit(EXIT_SUCCESS);
}
