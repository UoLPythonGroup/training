/** @file timer.h
 *  @author Matthew Daws
 *
 *  @section DESCRIPTION
 *  Simple class to time code snippets; header file
 */

/** Abstract Base Class to time a code snippet: overload `work`. */
class time_snippet {
public:
	/** Method to overload */
	virtual int work() =0;
	double total_time(int loops);
	double time(int loops);
	double time_auto();
	double best_auto(int trials);
};

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

