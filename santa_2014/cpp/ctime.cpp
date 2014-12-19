/** Test some things about ctime */

#include <iostream>
using std::endl;
using std::cout;

#include <ctime>
#include <stdexcept>

int main()
{
	check_ctime();

	time_t tt;
	time(&tt);

	cout << "Raw time read is: " << tt << endl;
	
	struct tm * ptm;
	ptm = gmtime(&tt);
	cout << "C library as UTC time: " << asctime(ptm) << endl;
	ptm = localtime(&tt);
	cout << "C library as local time: " << asctime(ptm) << endl;

	cout << "Test for 32-bit overflow issues" << endl;
	tt = 2147483647;
	for (int i=0; i<10; ++i) {
		ptm = gmtime(&tt);
		cout << "C library as UTC time: " << asctime(ptm);
		++tt;
	}
	
	return 0;
}
