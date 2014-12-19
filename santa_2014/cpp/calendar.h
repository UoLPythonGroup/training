/** calendar.h
 *
 *  Basic calendar functionality for the Elf problem.  Translate a year/month/day/hour/minute
 *  numbers into minutes since start of 2014, and back again.
 *  Don't believe this can be done in pure C++; probably easy with Boost or similar.
 */
 
#ifndef __calendar_h
#define __calendar_h

// I do quite like the chrono header, and the Duration idea!
#include <chrono>
#include <string>

class Elf_calendar {
public:
	struct tm {
		int year, month, day, hour, minute;
		tm() : year{0}, month{0}, day{0}, hour{0}, minute{0} {};
		tm(int y, int mo=1, int d=1, int h=0, int m=0) : year{y}, month{mo}, day{d}, hour{h}, minute{m} {};
   	std::string print();
	};
	Elf_calendar(std::chrono::minutes from2014) : tf2{from2014} {};
	Elf_calendar(tm);
	std::chrono::minutes time_from_2014()const { return tf2; }
	tm time_split()const;
private:
	std::chrono::minutes tf2;
	static constexpr int minutes_a_year[2] = { 525600, 527040 };
	static constexpr std::chrono::hours one_day{24};
private:
	bool is_leap_year(int year)const;
   int leap_year_count(int start, int end)const;
   int day_count(int startyear, int endyear)const;
   int num_div_half_open_int(int start, int end, int gap)const;
   int days_a_month(int year, int month)const;
};

#endif // __calendar_h
