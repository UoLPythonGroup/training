/** Elf class
 *
 *  Currently a basic Elf class to support the scorer.
 */

#include <tuple>
#include <stdexcept>
#include <cmath>

namespace Elfns {

// Globals, but inside a namespace, so not evil I think
constexpr int daystarts = 9*60;
constexpr int dayends = 19*60;
constexpr int minsday = 24*60;
constexpr int sanctionedminsday = dayends - daystarts;
constexpr int unsanctionedminsday = minsday - sanctionedminsday;

/** Input: duration, place, start, end
 *   where start--end represents an interval of time, place is the current place in the
 *   day, and duration is the total duration.
 *   Intersects [place, place+duration] with the interval, and returns the 'delta' which
 *   is the maximum we can add to place to either get duration==0 or to reach the end of
 *   the interval; returns 0 if no intersection
 */
int ssu_spliter(int duration, int place, int start, int end);

/** Split time period into sanctioned and unsanctioned minutes
 *   Input: starttime, integer, time since 2014 in minutes
 *     duration, integer, time in minutes
 *   Returns: (sanctioned, unsanctioned)
 */
std::tuple<int,int> split_sanctioned_unsanctioned(int starttime, int duration);

class ElfExcept : public std::runtime_error
{
public:
   ElfExcept(const char *str) : std::runtime_error(str) {}
};

class Elf
{
public:
   Elf(int i) : id{i}, productivity{1.0}, available_to_work{9*60} {}
   int do_task(int starttime, int duration);
	int time_to_do_task(int tasklength)const;
	int next_available()const { return available_to_work; }
private:
   double productivity;
   int id;
   int available_to_work;
};

} // Elfns namespace
