/** @file threaded_MC_pi.cpp
 *  @author Matthew Daws
 *
 *  @section DESCRIPTION
 *  Simple, manual, threads in C++ to do a Monte Carlo calculation of Pi.
 *  g++ -std=c++11 threaded_MC_pi.cpp -o threaded_MC_pi.exe
 *
 * i5-3210m, fixed at 2.5GHz, (2 cores, 4 threads) no optimisation
 * iters      single-thread   1       2       3       4
 * 10^5       3.92e-2         3.93e-2 2.05e-2 1.85e-2 2.03e-2
 * 10^6       0.391           0.390   0.204   0.170   0.138
 * 10^7       3.91            3.90    2.11    1.64    1.38
 * 10^8       39.1            39.0    21.5    16.2    13.8
 *
 * With -O2 -march,mtune=native
 * 10^6       1.44e-2         1.47e-2 8.55e-3 9.82e-3 6.85e-3
 * 10^7       0.144           0.145   0.085   0.078   0.067
 * 10^8       1.44            1.45    0.809   0.703   0.668
 * 10^9       14.4            14.4    8.31    6.97    6.66
 * Of note is that pi = 3.14166 (!) even with 10^9
 *
 * WHY don't we get a 2x speedup?  Well, running two instances of the single-threaded version
 * at 10^8 takes about 1.67 seconds, so about 16% longer than expected.  This "predicts" the
 * 2 threaded version at 0.835 seconds, which is more than we see.
 *
 * Varying the PRND used can change this: e.g. knuth_b is slow, but 2 threads is actually
 * slightly faster than 50% of single-threaded speed.  An allegedly known problem with the
 * Mersenne twister is it's large state space and knock-on effect on caches.
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


#include <future>
#include <thread>
#include <vector>

/** Use multi-threading.  If threads<0 then simulates via sequential calls. */
float mc_pi_threaded(unsigned int iters, int num_threads)
{
	if ( num_threads < 0 )
	{
		num_threads = -num_threads;
		float pi = 0.0;
		unsigned int start = 0;
		for (int i=0; i<num_threads; ++i) {
			// This sort of logic is over-kill here, but would be useful if we were walking
			// an array or similar.
			unsigned int end = start + iters / num_threads;
			if ( i == num_threads-1 ) { end = iters; }
			pi += mc_pi(end-start, i+1234);
			start = end;
		}
		return pi / static_cast<float>(num_threads);
	}

	using thread_type = std::future<float>;
	std::vector<thread_type> threads;
	unsigned int start = 0;
	for (int i=0; i<num_threads; ++i) {
		unsigned int end = start + iters / num_threads;
		if ( i == num_threads-1 ) { end = iters; }
		auto task = [start, end, i](){ return mc_pi(end-start, i+1234); };
		threads.push_back( std::async(std::launch::async, task) );
		start = end + 1;
	}
	float pi = 0.0;
	for (int i=0; i<num_threads; ++i) {
		pi += threads[i].get();
	}
	return pi / static_cast<float>(num_threads);
}


#include <iostream>
using std::cout;
using std::endl;
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
