#include <iostream>
using std::endl;
using std::cout;

#include "calendar.h"

int main()
{
   cout << "'2014 1 2 3 4' -> " << Elf_calendar(Elf_calendar::tm{2014,1,2,3,4}).time_from_2014().count() << endl;
   cout << "'2014 11 12 13 14' -> " << Elf_calendar(Elf_calendar::tm{2014,11,12,13,14}).time_from_2014().count() << endl;
   cout << "'2016 5 6 10 53' -> " << Elf_calendar(Elf_calendar::tm{2016,5,6,10,53}).time_from_2014().count() << endl;
   cout << "'2018 8 9 10 11' -> " << Elf_calendar(Elf_calendar::tm{2018,8,9,10,11}).time_from_2014().count() << endl;
   cout << "'2020 2 15 10 0' -> " << Elf_calendar(Elf_calendar::tm{2020,2,15,10,0}).time_from_2014().count() << endl;
   cout << "'2020 3 15 10 0' -> " << Elf_calendar(Elf_calendar::tm{2020,3,15,10,0}).time_from_2014().count() << endl;
   cout << "'2200 9 10 23 55' -> " << Elf_calendar(Elf_calendar::tm{2200,9,10,23,55}).time_from_2014().count() << endl;
   cout << "'2450 8 7 6 5' -> " << Elf_calendar(Elf_calendar::tm{2450,8,7,6,5}).time_from_2014().count() << endl;

   cout << "1624 -> " << Elf_calendar(std::chrono::minutes{1624}).time_split().print() << endl;
   cout << "454394 -> " << Elf_calendar(std::chrono::minutes{454394}).time_split().print() << endl;
   cout << "1233293 -> " << Elf_calendar(std::chrono::minutes{1233293}).time_split().print() << endl;
   cout << "2421251 -> " << Elf_calendar(std::chrono::minutes{2421251}).time_split().print() << endl;
   cout << "3220440 -> " << Elf_calendar(std::chrono::minutes{3220440}).time_split().print() << endl;
   cout << "3262200 -> " << Elf_calendar(std::chrono::minutes{3262200}).time_split().print() << endl;
   cout << "98190715 -> " << Elf_calendar(std::chrono::minutes{98190715}).time_split().print() << endl;
   cout << "229628525 -> " << Elf_calendar(std::chrono::minutes{229628525}).time_split().print() << endl;

   cout << "16753 -> " << Elf_calendar(std::chrono::minutes{16753}).time_split().print() << endl;
   cout << "1242123 -> " << Elf_calendar(std::chrono::minutes{1242123}).time_split().print() << endl;
   cout << "5428452 -> " << Elf_calendar(std::chrono::minutes{5428452}).time_split().print() << endl;
   cout << "14523 -> " << Elf_calendar(std::chrono::minutes{14523}).time_split().print() << endl;
}


