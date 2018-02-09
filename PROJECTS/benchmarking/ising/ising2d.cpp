/** @file ising2d.cpp
 *  @author Matthew Daws
 *
 *  @section DESCRIPTION
 *  Getting the Ising model working, for now.
 */

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
#include <random>

#include <fstream>




#include <chrono>

/** Template function to do simple timing: best used with a lambda. */
template<typename Func>
double timeit(int loops, Func func)
{
	std::chrono::steady_clock::time_point t1,t2;
	t1 = std::chrono::steady_clock::now();
	for (int n=0; n<loops; ++n) {
		func();
	}
	t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_span;
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	return time_span.count() / static_cast<double>(loops);
}



/** Class for basic 2D Ising simulation */

using IsingType = char;

class TwoDIsing {
public:
	TwoDIsing(int L);
	void update();
	void writeout(std::ostream &str)const;
private:
	std::vector<IsingType> lattice;
	int length;
	std::mt19937 rnd_gen;
	std::uniform_int_distribution<int> rnd_pos;
	std::uniform_int_distribution<int> rnd_bits;
	
	IsingType inline get_state(int x, int y)const {
		if ( x==-1 ) { x=length-1; }
		if ( x==length ) { x=0; }
		if ( y==-1 ) { y=length-1; }
		if ( y==length ) { y=0; }
		return lattice[x+y*length];
	};
	int num_nbours_equal(const int x, const int y)const;
};

TwoDIsing::TwoDIsing(int L)
	: length{L}, lattice(L*L), rnd_pos(0,L-1), rnd_bits(0,1)
{
	for (auto &x : lattice) {
		x = rnd_bits(rnd_gen);
	}
}

int TwoDIsing::num_nbours_equal(const int x, const int y)const
{
	int count = 0;
	auto state = get_state(x,y);
	count += ( state == get_state(x-1,y) );
	count += ( state == get_state(x+1,y) );
	count += ( state == get_state(x,y-1) );
	count += ( state == get_state(x,y+1) );
	return count;
}

void TwoDIsing::update()
{
	int x = rnd_pos(rnd_gen);
	int y = rnd_pos(rnd_gen);
	//cout << "Updating point (" << x << "," << y << ")" << endl;
	int count = num_nbours_equal(x,y);
	if ( count < 2 or ( count == 2 and rnd_bits(rnd_gen) == 0 ) ) {
		lattice[x + y*length] = 1 - lattice[x + y*length];
	}
}

void TwoDIsing::writeout(std::ostream &str)const
{
	str << length << endl;
	for (int y=0; y<length; ++y) {
		for (int x=0; x<length; ++x) {
			str << static_cast<int>( lattice[x+y*length] );
			if ( x<length-1 ) { str << ", "; }
		}
		str << endl;
	}
}



int make_files()
{
	TwoDIsing lat{50};

	{
		std::ofstream file("start.ising.txt");
		if ( !file ) { return -1; }
		lat.writeout(file);
	}
	
	for (int n=0; n<1000; ++n) { lat.update(); }

	{
		std::ofstream file("next.ising.txt");
		if ( !file ) { return -1; }
		lat.writeout(file);
	}
	
	for (int n=0; n<100000; ++n) { lat.update(); }

	{
		std::ofstream file("end.ising.txt");
		if ( !file ) { return -1; }
		lat.writeout(file);
	}

	return 0;
}

void bench(TwoDIsing &lat, int reps)
{
	for (int n=0; n<reps; ++n) { lat.update(); }
}

int main()
{
	// Note that as we don't change `seed` in the random number generator,
	//   this runs the _same_ test each time!
	TwoDIsing lat{50};
	for (int k=0; k<20; ++k) {
		cout << timeit(1,[&lat](){ bench(lat,1000000); }) << endl;
	}
}

