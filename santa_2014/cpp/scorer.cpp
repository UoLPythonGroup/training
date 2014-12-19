/** C++ version of the scorer for the Elf problem
 *
 */

#include <vector>
#include <cmath>
#include <iostream>
using std::endl;
using std::cout;
#include <iomanip>

#include "load_npy.h"
#include "load_toys.h"
//#include "calendar.h"
#include "elf.h"
using namespace Elfns;

struct Elf_Job {
   int toyid, elfid, starttime, duration;
};

int main()
{
	constexpr int numelves = 900;
	std::vector<Elf> elves;
	for (int i=0; i<numelves; ++i) {
		elves.push_back( Elf{i+1} );
	}
	auto toys = load_toys_npy("../toys_rev2.npy");
	Load_npy sub_npy("../sampleSubmission_rev2.npy");
	int numtoys = toys.size();
	if ( sub_npy.get_shape()[0] != numtoys ) {
		cout << "Have " << numtoys <<" toys but " << sub_npy.get_shape()[0] << " jobs!" << endl;
		return 1;
	}
	std::vector<bool> toydone(numtoys, false);
	int maxtime = 0;
	for (int i=0; i<numtoys; ++i) {
		Elf_Job nextjob;
		sub_npy.readrow(&nextjob);
		if ( nextjob.elfid < 1 or nextjob.elfid > numelves ) {
			cout << "Row " << i << ": invalid elfid:" << nextjob.elfid << endl;
			return 1;
		}
		auto *ourelf = &elves[nextjob.elfid-1];
		if ( nextjob.toyid < 1 or nextjob.toyid > numtoys ) {
			cout << "Row " << i << ": invalid toyid:" << nextjob.toyid << endl;
			return 1;
		}
		if ( toydone[nextjob.toyid-1] ) {
			cout << "Row " << i << ": invalid toyid:" << nextjob.toyid << " as toy already built!" << endl;
			return 1;
		}
		auto ourtoy = toys[nextjob.toyid-1];
		if ( nextjob.starttime < 0 ) {
			cout << "Row " << i << ": invalid starttime:" << nextjob.starttime << endl;
			return 1;
		}
		if ( ourelf->next_available() > nextjob.starttime ) {
			cout << "Row " << i << ": invalid starttime:" << nextjob.starttime;
			cout << " as elf not available then!" << endl;
			return 1;
		}
		if ( nextjob.starttime < ourtoy.arrival_time ) {
			cout << "Row " << i << ": invalid starttime:" << nextjob.starttime;
			cout << " as toy has not arrived yet!" << endl;
			return 1;
		}
		if ( nextjob.duration < 0 ) {
			cout << "Row " << i << ": invalid duration:" << nextjob.duration << endl;
			return 1;
		}
		if ( ourelf->time_to_do_task(ourtoy.duration) > nextjob.duration ) {
			cout << "Row " << i << ": invalid duration:" << nextjob.duration;
			cout << " as Elf cannot complete task in this time." << endl;
			return 1;
		}
		if ( ourelf->time_to_do_task(ourtoy.duration) < nextjob.duration ) {
			cout << "Row " << i << ": odd duration:" << nextjob.duration;
			cout << " as Elf could complete this faster." << endl;
		}
		int timetaken = ourelf->do_task(nextjob.starttime, ourtoy.duration);
		maxtime = std::max(maxtime, nextjob.starttime + timetaken);
		toydone[nextjob.toyid-1] = true;
		if ( i % 1000000 == 0 ) { cout << i/100000 << "  \r"; }
	}
	cout << "All assigned tasks done!" << endl;
	for (int i=0; i<numtoys; ++i) {
		if ( !toydone[i] ) {
			cout << "toy " << i+1 << " not built!";
			return 1;
		}
	}
	cout << "All toys built!" << endl;
	cout << "Time taken = " << maxtime << " minutes, so score = ";
	cout << std::setprecision(20);
	cout << static_cast<double>(maxtime) * log(static_cast<double>(numelves)+1.0) << endl;
}
