/** @file: fibo_native_funcs.cpp
 *  @author: Matthew Daws
 *
 * Put actual functions to be tested in a separate file to stop compiler optimising it all away!
 */

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
#include <utility>

/** Assuming we're on a 64-bit, overflows for n > 94 */
void fibo64(const int n, bool print)
{
	unsigned long long a = 0;
	unsigned long long b = 1;
	
	for (int i=0; i<n-2; ++i)
	{
		a += b;
		std::swap(a,b);
	}

	if ( print ) {
		cout << b << endl;
	}
}

std::ostream& operator<<(std::ostream& strm, unsigned __int128 num)
{
	std::vector<int> digits;
	while ( num!=0 ) {
		digits.push_back( num%10 );
		num /= 10;
	};
	for (auto rit=digits.rbegin(); rit!=digits.rend(); ++rit) {
		strm << *rit;
	}
	return strm;
}

/** Assuming we're on a 128-bit, overflows for n > 187 */
void fibo128(const int n, bool print)
{
	unsigned __int128 a = 0;
	unsigned __int128 b = 1;
	
	for (int i=0; i<n-2; ++i)
	{
		a += b;
		std::swap(a,b);
	}

	if ( print ) {
		cout << b << endl;
	}
}
