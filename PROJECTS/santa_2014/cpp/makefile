CC = g++
//CPPFLAGS = -std=c++11 -D_GLIBCXX_DEBUG
CPPFLAGS = -std=c++11 -O2 -march=native -mtune=native

all: load_toys_test.exe calendar_demo.exe scorer.exe

load_toys_test.exe : load_toys_test.o load_toys.o calendar.o load_npy.o
	g++ $(CPPFLAGS) $^ -o $@

calendar_demo.exe : calendar_demo.o calendar.o
	g++ $(CPPFLAGS) $^ -o $@

scorer.exe : scorer.o load_toys.o load_npy.o elf.o calendar.o
	g++ $(CPPFLAGS) $^ -o $@



calendar.o : calendar.cpp calendar.h
	g++ -c $(CPPFLAGS) $< -o $@

calendar_demo.o : calendar_demo.cpp calendar.o
	g++ -c $(CPPFLAGS) $< -o $@

load_toys.o : load_toys.cpp load_toys.h calendar.o
	g++ -c $(CPPFLAGS) $< -o $@

load_toys_test.o : load_toys_test.cpp load_toys.o calendar.o
	g++ -c $(CPPFLAGS) $< -o $@

load_npy.o : load_npy.cpp load_npy.h
	g++ -c $(CPPFLAGS) $< -o $@

scorer.o : scorer.cpp load_toys.o load_npy.o elf.o
	g++ -c $(CPPFLAGS) $< -o $@

elf.o : elf.cpp
	g++ -c $(CPPFLAGS) $< -o $@
