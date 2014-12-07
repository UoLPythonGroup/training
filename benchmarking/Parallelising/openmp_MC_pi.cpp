/** @file openmp_MC_pi.cpp
 *  @author Matthew Daws
 *
 *  @section DESCRIPTION
 *  Same as before, but now using openmp
 *  g++ -std=c++11 -fopenmp -O2 -march=native -mtune=native openmp_MC_pi.cpp -o openmp_MC_pi.exe
 *
 *  Performance (as we might hope) is the same as for the manual threads version.
 */


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


#include <random>

float mc_pi(unsigned int count, int seed)
{
	// Mersenne twister
	std::mt19937 rnd_gen;
	rnd_gen.seed(seed);
	// U[0,1] distribution
	std::uniform_real_distribution<float> dis(0,1);
	unsigned int in_circle = 0;
	for (unsigned int i=0; i<count; ++i) {
		float x = dis(rnd_gen);
		float y = dis(rnd_gen);
		if ( x*x + y*y <= 1.0 ) { 
			++in_circle;
		}
	}
	return 4.0 * static_cast<float>(in_circle) / static_cast<float>(count);
}


#include "omp.h"
#include <iostream>
using std::cout;
using std::endl;

class dummy {
public:
	dummy(const int i) { local_i = i; }
	int get() {
		if ( local_i==0 ) { return 1; } else { return 0; }
	}
private:
	int local_i;
};

/** Use multi-threading.  If threads<0 then simulates via sequential calls. */
float mc_pi_threaded(unsigned int count, int num_threads)
{
	if ( num_threads < 0 )
	{
		num_threads = -num_threads;
		// Slight guess as to what "schedule(static)" does...
		unsigned int start=0;
		float pi = 0.0;
		for (int i=0; i<num_threads; ++i) {
			unsigned int end = start + count/num_threads;
			if ( i == num_threads-1 ) { end = count; }
			pi += mc_pi(end-start, 1234+i);
			start = end;
		}
		return pi/static_cast<float>(num_threads);
	}

	unsigned int in_circle = 0;
	omp_set_num_threads(num_threads);
	#pragma omp parallel
	{
		int thread_id = omp_get_thread_num();
		// Give each thread its own PRND with a unique seed (or else each thread produces the
		// _same_ sequence of "random" numbers!
		std::mt19937 rnd_gen;
		rnd_gen.seed(1234+thread_id);
		std::uniform_real_distribution<float> dis(0,1);
		#pragma omp for schedule(static) reduction (+:in_circle)
		for (unsigned int i=0; i<count; ++i) {
			float x = dis(rnd_gen);
			float y = dis(rnd_gen);
			if ( x*x + y*y <= 1.0 ) { 
				++in_circle;
			}
		}
	}
	return 4.0 * static_cast<float>(in_circle) / static_cast<float>(count);
}


#include <string>
#include <sstream>

int main(int argc,char *argv[])
{
	// Gracefully handle command line options or lack thereof
	if ( argc < 3 )
	{
		cout << "Usage: Iterations Threads" << endl;
		cout << "If Threads==0 then run single-threaded test." << endl;
		cout << "If Threads<0 then run multi-threaded simulation.  Should give same result" << endl;
		cout << "  due to manual setting of PRND seed." << endl;
		return 1;
	}
	unsigned int iters;
	std::stringstream(std::string(argv[1])) >> iters;
	int num_threads;
	std::stringstream(std::string(argv[2])) >> num_threads;

	cout << "Loops: " << iters << endl;

	if ( num_threads==0 ) {
		cout << "Single-threaded test:" << endl;
		for (int i=0; i<5; ++i) {
			float result;
			auto command = [&result,iters](){ result = mc_pi(iters,1234); }; // Lambda command
			cout << "Time: " << timeit(1,command);
			cout << " gave pi=" << result << endl;
		}
		return 0;
	}
	
	cout << "Now testing with " << num_threads << " threads." << endl;
	for (int i=0; i<5; ++i) {
		float result;
		auto command = [&result,iters,num_threads](){ result = mc_pi_threaded(iters, num_threads); };
		cout << "Time: " << timeit(1,command);
		cout << " gave pi=" << result << endl;
	}
	
	return 0;
}
