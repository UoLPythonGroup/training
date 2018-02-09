/** Simple class to do date type processing.
 *
 *  The reason for writing this is that the C++ standard library seems to provide
 *  no real calendar handling functions, except for locale related things which
 *  interface directly to the C library.
 *  The C library can suffer from the "Year 2038" problem,
 *    https://en.wikipedia.org/wiki/Year_2038_problem
 *  In principle we could use the tm structure, and time_t, but while this would work
 *  on my setup (second accuracy, 64-bit arithmetic type) there's a good chance it
 *  wouldn't be portable.
 *
 *  As we only need a rather easy algorithm, taking account of leap years, why not
 *  roll our own?
 */

#include "calendar.h"

#include <sstream>

// Need to define these
constexpr int Elf_calendar::minutes_a_year[2];
constexpr std::chrono::hours Elf_calendar::one_day;

int Elf_calendar::days_a_month(int year, int month)const
{
   if ( month==2 and is_leap_year(year) ) { return 29; }
	constexpr int days_a_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   if ( month < 1 or month > 12 ) { return 0; }
   return days_a_month[month-1];
}

bool Elf_calendar::is_leap_year(int year)const
{
	if ( year % 4 != 0 ) { return false; }
	if ( year % 100 != 0 ) { return true; }
	if ( year % 400 != 0 ) { return false; }
	return true;
}

Elf_calendar::Elf_calendar(tm t)
{
	tf2 = std::chrono::minutes{0};
	// Minimal validation
	if ( t.year < 2014 or t.month < 1 or t.month > 12 ) { return; }
	// Move to correct year counting leap years
   tf2 += one_day * day_count(2014, t.year);
	// Move to current month
	for (int current_month = 1; current_month < t.month; ++current_month) {
		tf2 += one_day * days_a_month(t.year,current_month);
	}
	// Now the rest
	tf2 += one_day * (t.day - 1);
	tf2 += std::chrono::hours{1} * t.hour;
	tf2 += std::chrono::minutes{1} * t.minute;
}

// Number of days in interval [start, end)
int Elf_calendar::day_count(int startyear, int endyear)const
{
   return (endyear - startyear) * 365 + leap_year_count(startyear, endyear);
}

// Number of leap years in interval [start, end)
int Elf_calendar::leap_year_count(int start, int end)const
{
	return num_div_half_open_int(start,end,4) - num_div_half_open_int(start,end,100)
		+ num_div_half_open_int(start,end,400);
}

// Helper function for leap_year_count()
int Elf_calendar::num_div_half_open_int(int start, int end, int gap)const
{
	start = (start + gap - 1) / gap; // Round up  504
	end = (end + gap - 1) / gap; // Half open interval  504
	if ( start >= end ) { return 0; }
	return end - start;
}

Elf_calendar::tm Elf_calendar::time_split()const
{
	tm time;
	if ( tf2.count() < 0 ) { return time; }
	std::chrono::minutes temp = tf2;
	// Minutes
	time.minute = (temp % std::chrono::hours{1}).count();
	temp -= std::chrono::minutes{1} * time.minute;
	// Hours
	time.hour = (std::chrono::duration_cast<std::chrono::hours>(temp % one_day)).count();
	temp -= std::chrono::hours{1} * time.hour;
	// Now days etc.
	int numdays = temp.count() / std::chrono::duration_cast<std::chrono::minutes>(one_day).count();
	time.year = 2014 + numdays / 365;
	while ( day_count(2014, time.year) > numdays ) { // Correct if problem due to leap years
      --time.year;
	}
   numdays -= day_count(2014, time.year);
   // Now month
   for (time.month = 1; numdays >= days_a_month(time.year, time.month); ++time.month) {
      numdays -= days_a_month(time.year, time.month);
   }
   time.day = numdays + 1;
   return time;
}

std::string Elf_calendar::tm::print()
{
   std::stringstream s;
   s << year << " " << month << " " << day << " " << hour << " " << minute;
   return s.str();
}
