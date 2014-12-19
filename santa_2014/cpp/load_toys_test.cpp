/** Load in the toy list manually from CSV, and by importing the Numpy file, and compare!
 *
 */

#include <iostream>
using std::endl;
using std::cout;

#include <fstream>
#include "load_toys.h"

int main()
{
   auto csv_toys = load_toys_csv("../toys_rev2.csv"); // Is this mingw showing its unix roots?
   cout << "Loading NPY file..." << endl;
   auto npy_toys = load_toys_npy("../toys_rev2.npy"); // Is this mingw showing its unix roots?
	cout << "Comparing..." << endl;
	if ( csv_toys.size() != npy_toys.size() ) {
	   cout << "Not even the same size!" << endl;
	   return 0;
	}
	for (int i = 0; i < csv_toys.size(); ++i) {
      if ( csv_toys[i].arrival_time != npy_toys[i].arrival_time
         or csv_toys[i].duration != npy_toys[i].duration ) {
         cout << i << " : not equal." << endl;
         return 0;
      }
	}
	cout << "Match!" << endl;
}


