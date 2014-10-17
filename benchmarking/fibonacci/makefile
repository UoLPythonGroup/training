CC = g++
CPPFLAGS = -std=c++11 -O3 -march=native -mtune=native

fibo_native.exe : fibo_native.o fibo_native_funcs.o
	$(CC) -o fibo_native.exe fibo_native.o fibo_native_funcs.o

fibo_cpp.exe : fibo_cpp.o
	$(CC) -o fibo_cpp.exe fibo_cpp.o -lgmpxx -lgmp

fibo_cpp.o : fibo_cpp.cpp

fibo_native.o : fibo_native.cpp

fibo_native_funcs.o : fibo_native_funcs.cpp

clean :
	-rm fibo_native_funcs.o fibo_native.o fibo_native.exe fibo_cpp.o fibo_cpp.exe
