/** Load in the toy list manually from CSV, and by importing the Numpy file, and compare!
 *
 */


#include "load_toys.h"
#include "load_npy.h"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include "calendar.h"

/** This version is perhaps the "cleanest" C++ version.
 *  Takes around 13 secs.
 *
 *  Some commentary e.g. here: http://stackoverflow.com/questions/23304177/c-alternative-for-parsing-input-with-sscanf
 */
std::vector<toy_pair> load_toys_csv_old(std::string filename)
{
   std::vector<toy_pair> toys;
   std::ifstream file(filename);
   if ( !file.good() ) {
      return toys;
   }
   std::string row;
   std::getline(file, row); // Header, discard
   int rownum = 1;
   while ( true )
   {
      int toyid;
      std::string arrival_date;
      toy_pair tp;

      std::getline(file, row);
      if ( file.eof() ) { break; }
      if ( !file.good() ) { return toys; } // Same effect, but different semantics, if we wanted...
      std::stringstream ss{row};
      char dummy;
      ss >> toyid >> dummy;
      Elf_calendar::tm time;
      ss >> time.year >> time.month >> time.day >> time.hour >> time.minute;
      ss >> dummy >> tp.duration;      
      Elf_calendar el(time);
      tp.arrival_time = el.time_from_2014().count();
      
      if ( rownum != toyid ) {
         throw std::runtime_error("load_toys_csv_c_old: unexpected toy number!");
      }
      toys.push_back(tp);
      ++rownum;
   }
   return toys;
}

int our_stoi(std::string &str, std::string::size_type start, std::string::size_type end)
{
   return std::stoi(str.substr(start, end));
}

int find_extract(std::string &allfile, const char split, std::string::size_type &start)
{
   auto end = allfile.find_first_of(split, start);
   int ret;
   if ( end == std::string::npos )
   {
      ret = our_stoi(allfile,start, std::string::npos);
      start = std::string::npos;
   } else {
      ret = our_stoi(allfile,start, end-start);
      start = end + 1;
   }
   return ret;
}

/** This version loads the whole file, and then manually processes the resulting string.
 *  Takes around 9 secs.
 */
std::vector<toy_pair> load_toys_csv(std::string filename)
{
   std::vector<toy_pair> toys;
   std::ifstream file(filename);
   if ( !file.good() ) {
      return toys;
   }
   std::string row;
   std::getline(file, row); // Header, discard
   std::string allfile;
   std::getline(file, allfile, char(0));
   
   // Faster but more tedious!
   /*std::ifstream file(filename, std::ios::binary);
   auto oldpos = file.tellg();
   file.seekg(0,std::ios_base::end);
   auto length = file.tellg() - oldpos;
   cout << "Length: "<< length << "   " << endl;
   file.seekg(oldpos,std::ios_base::beg);
   allfile.resize(length);
   file.read(&allfile[0], length);*/
   
   std::string::size_type start = 0;
   int rownum = 1;
   toys.reserve(10000000);
   while ( start < allfile.length() )
   {
      int toyid;
      Elf_calendar::tm time;
      toy_pair tp;

      toyid = find_extract(allfile, ',', start);
      time.year = find_extract(allfile, ' ', start);
      time.month = find_extract(allfile, ' ', start);
      time.day = find_extract(allfile, ' ', start);
      time.hour = find_extract(allfile, ' ', start);
      time.minute = find_extract(allfile, ',', start);
      tp.duration = find_extract(allfile, 0x0a, start); // Up to new line

      Elf_calendar el(time);
      tp.arrival_time = el.time_from_2014().count();
      
      if ( rownum != toyid ) {
         throw std::runtime_error("load_toys_csv: unexpected toy number!");
      }
      toys.push_back(tp);
      ++rownum;
   }
   return toys;
}

/** This would be the classic C version.  Surprisingly is also the slowest, at 23 seconds.
 */
#include <cstdio>
std::vector<toy_pair> load_toys_csv_c(std::string filename)
{
   std::vector<toy_pair> toys;
   FILE* f = fopen(filename.c_str(), "r");
   if ( ferror(f) ) { return toys; }
   char buf[100];
   fscanf(f, "%80s", buf);
   int rownum = 1;
   while ( !ferror(f) )
   {
      int toyid;
      Elf_calendar::tm time;
      toy_pair tp;
      fscanf(f, "%d,%d %d %d %d %d,%d", &toyid, &time.year, &time.month, &time.day, &time.hour, &time.minute, &tp.duration);
      if ( feof(f) ) { break; }
      Elf_calendar el(time);
      tp.arrival_time = el.time_from_2014().count();
      
      if ( rownum != toyid ) {
         throw std::runtime_error("load_toys_csv_c: unexpected toy number!");
      }
      toys.push_back(tp);
      ++rownum;
   }
   fclose(f);
   return toys;
}

std::vector<toy_pair> load_toys_npy(std::string filename)
{
	return load_numpy_structured_rows<toy_pair>(filename, 2, "i4");
}
