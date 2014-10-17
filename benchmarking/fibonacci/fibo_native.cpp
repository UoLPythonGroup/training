/** @file: fibo_native.cpp
 *  @author: Matthew Daws
 *
 *  C++ fibonnaci benchmark, using built in 64-bit and 128-bit arithmetic.

 *  Compile with g++ -std=c++11 -O3 -march=native -mtune=native fibo_native.cpp -o fibo_native.exe
 */

#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <sstream>

// In other file
void fibo64(const int n, bool print);
void fibo128(const int n, bool print);

#include "timer.hpp"

int main(int argc, char** argv)
{
	if ( argc > 1 )
	{
		int num;
		std::stringstream(std::string(argv[1])) >> num;
		cout << "64bit version: ";
		fibo64(num,true);
		cout << "128bit version: ";
		fibo128(num,true);
		return 0;
	}

	cout << "10000000 loops for n=94, 64-bit integers:" << endl;
	cout << timeit(10000000,[](){ fibo64(94,false); }) << endl;
	cout << timeit(10000000,[](){ fibo64(94,false); }) << endl;
	cout << timeit(10000000,[](){ fibo64(94,false); }) << endl;
	cout << "10000000 loops for n=94, 128-bit integers:" << endl;
	cout << timeit(10000000,[](){ fibo128(94,false); }) << endl;
	cout << timeit(10000000,[](){ fibo128(94,false); }) << endl;
	cout << timeit(10000000,[](){ fibo128(94,false); }) << endl;
	cout << "1000000 loops for n=187, 128-bit integers:" << endl;
	cout << timeit(1000000,[](){ fibo128(187,false); }) << endl;
	cout << timeit(1000000,[](){ fibo128(187,false); }) << endl;
	cout << timeit(1000000,[](){ fibo128(187,false); }) << endl;
	
	return 0;
}
