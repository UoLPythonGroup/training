/** Load in the toy list manually from CSV, and by importing the Numpy file, and compare!
 *
 */

#include <vector>
#include <string>

struct toy_pair {
   int arrival_time;
   int duration;
};

/** This version is perhaps the "cleanest" C++ version.
 *  Takes around 13 secs.
 *
 *  Some commentary e.g. here: http://stackoverflow.com/questions/23304177/c-alternative-for-parsing-input-with-sscanf
 */
std::vector<toy_pair> load_toys_csv_old(std::string filename);

/** This version loads the whole file, and then manually processes the resulting string.
 *  Takes around 9 secs.
 */
std::vector<toy_pair> load_toys_csv(std::string filename);

/** This would be the classic C version.  Surprisingly is also the slowest, at 23 seconds.
 */
std::vector<toy_pair> load_toys_csv_c(std::string filename);

std::vector<toy_pair> load_toys_npy(std::string filename);
