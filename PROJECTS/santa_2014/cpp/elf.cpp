/** Elf class
 *
 *  Currently a basic Elf class to support the scorer.
 */

#include "elf.h"
using namespace Elfns;

/** Input: duration, place, start, end
 *   where start--end represents an interval of time, place is the current place in the
 *   day, and duration is the total duration.
 *   Intersects [place, place+duration] with the interval, and returns the 'delta' which
 *   is the maximum we can add to place to either get duration==0 or to reach the end of
 *   the interval; returns 0 if no intersection
 */
int Elfns::ssu_spliter(int duration, int place, int start, int end)
{
    if ( start <= place and place < end ) {
        return std::min(duration, end - place);
    }
    return 0;
}

/** Split time period into sanctioned and unsanctioned minutes
 *   Input: starttime, integer, time since 2014 in minutes
 *     duration, integer, time in minutes
 *   Returns: (sanctioned, unsanctioned)
 */
std::tuple<int,int> Elfns::split_sanctioned_unsanctioned(int starttime, int duration)
{
   int san = 0;
   int unsan = 0;
   // In any 24 hour period, always the same number of sanctioned and unsanctioned minutes
   // so deal with hour days first, and then with remainder
   int wholedays = duration / minsday;
   san += wholedays * sanctionedminsday;
   unsan += wholedays * unsanctionedminsday;
   duration -= wholedays * minsday;
   // So now duration < A Day, but still might overlap everything
   // Only care about starttime relative to current day now
   int day_starttime = starttime % minsday;
   while ( duration > 0 )
   {
      // Before start of day
      int delta = ssu_spliter(duration, day_starttime, 0, daystarts);
      unsan += delta;
      day_starttime = (day_starttime + delta) % minsday;
      duration -= delta;
      // Working hours
      delta = ssu_spliter(duration, day_starttime, daystarts, dayends);
      san += delta;
      day_starttime = (day_starttime + delta) % minsday;
      duration -= delta;
      // After working hours
      delta = ssu_spliter(duration, day_starttime, dayends, minsday);
      unsan += delta;
      day_starttime = (day_starttime + delta) % minsday;
      duration -= delta;
   }
   return std::pair<int,int>(san,unsan);
}

/** Get that Elf to work!
 *   Input: starttime, and duration as nominal time task takes.
 *   Returns the actual time the elf was working, taking account of elf productivity.
 */
int Elfns::Elf::do_task(int starttime, int duration)
{
   if ( starttime < available_to_work ) {
      throw ElfExcept("Elf not available to work.");
   }
   int actual_duration = time_to_do_task(duration);
   int san, unsan;
   // TODO: Does this do what I think it does?
   std::tie(san,unsan) = split_sanctioned_unsanctioned(starttime, actual_duration);
   available_to_work = starttime + actual_duration;
   // Update productivity
   productivity = pow(1.02,static_cast<double>(san)/60.0) * pow(0.9,static_cast<double>(unsan)/60.0) * productivity;
   productivity = std::max(productivity, 0.25);
   productivity = std::min(productivity, 4.0);
   // Work out rest time needed
   // Copied this logic from the official Python code, because of "bug"/"feature".
   if ( unsan > 0 ) {
      int num_days_since_jan1 = available_to_work / minsday;
      int rest_time = unsan;
      int rest_time_in_working_days = rest_time / sanctionedminsday;
      int rest_time_remaining_minutes = rest_time % sanctionedminsday;
      int local_start = available_to_work % minsday;
      if ( local_start < daystarts ) {
         local_start = daystarts;
      }  else {
         if ( local_start > dayends ) {
            ++num_days_since_jan1;
            local_start = daystarts;
         }
      }
      if ( local_start + rest_time_remaining_minutes > dayends ) {
         ++rest_time_in_working_days;
         rest_time_remaining_minutes -= (dayends - local_start);
         local_start = daystarts;
      }
      int total_days = num_days_since_jan1 + rest_time_in_working_days;
      available_to_work = total_days * minsday + local_start + rest_time_remaining_minutes;
   }
   return actual_duration;
}

int Elfns::Elf::time_to_do_task(int tasklength)const
{
	return ceil(static_cast<double>(tasklength) / productivity);
}
