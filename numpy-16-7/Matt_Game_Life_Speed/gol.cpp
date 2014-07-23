// Simple game of life speed test

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <chrono>

// Simple, but reproducible, rng
class myrand {
public:
	myrand() : state(1234) {}
	myrand(int start) : state(start) {}
	unsigned int get();
	int getbit() { return (get()>>20)&1; }
private:
	unsigned long long state;
};

// Should be what glibc uses according to Wikipedia.  I think this is a highly
// dubious claim; this is an awful PRNG!
unsigned int myrand::get()
{
	state = (state * 1103515245 + 12345) & 2147483647;
	return static_cast<unsigned int>(state);
}


template <typename T>
class game_of_life;

template <typename T>
std::ostream& operator<<(std::ostream& strm, const game_of_life<T> gol);

// Game of life class
template <typename T>
class game_of_life {
public:
	game_of_life(const int rows, const int cols);
	void set(const int row, const int col,const bool state);
	int get(const int row, const int col)const;
	void iterate();
	void init_random(myrand &rng);
	friend std::ostream& operator<< <>(std::ostream& strm, const game_of_life<T> gol);
private:
	std::vector<T> grid, newgrid;
	int numrows, numcols;
	int getwrap(int row, int col)const;
};

template <typename T>
std::ostream& operator<<(std::ostream& strm, const game_of_life<T> gol)
{
	strm << "Size of grid: " << gol.numrows << " rows by " << gol.numcols <<" cols" << std::endl;
	for (int r=0; r<gol.numrows; ++r) {
		for (int c=0; c<gol.numcols; ++c) {
			strm << gol.get(r,c) << " ";
		}
		strm << std::endl;
	}
	return strm;
}

template <typename T>
game_of_life<T>::game_of_life(const int rows, const int cols)
	: numrows{rows}, numcols{cols}, grid(rows*cols), newgrid(rows*cols)
{
}

template <typename T>
void game_of_life<T>::set(const int row, const int col,const bool state)
{
	grid[row*numcols + col] = (state ? 1 : 0);
}

template <typename T>
int game_of_life<T>::get(const int row, const int col)const
{
	return grid[row*numcols + col];
}

template <typename T>
void game_of_life<T>::init_random(myrand &rng)
{
	for (auto it=grid.begin(); it!=grid.end(); ++it) {
		*it = rng.getbit();
	}
}

// Implements periodic boundary, so messy
template <typename T>
int game_of_life<T>::getwrap(int row, int col)const
{
	if ( row < 0 ) { row += numrows; }
	if ( row >= numrows ) { row -= numrows; }
	if ( col < 0 ) { col += numcols; }
	if ( col >= numcols ) { col -= numcols; }
	return get(row,col);
}

template <typename T>
void game_of_life<T>::iterate()
{
	/*
	for (int r=0; r<numrows; ++r) {
		for (int c=0; c<numcols; ++c) {
			int n = getwrap(r,c-1) + getwrap(r,c+1)
				+ getwrap(r-1,c-1) + getwrap(r-1,c) + getwrap(r-1,c+1)
				+ getwrap(r+1,c-1) + getwrap(r+1,c) + getwrap(r+1,c+1);
			newgrid[r*numcols+c] = ( n==3 or ( n==2 and getwrap(r,c)==1 ) );
		}
	}
	std::swap(newgrid,grid);
	return;
	*/
	// (0,c)
	for (int c=0; c<numcols; ++c) {
		constexpr int r = 0;
		int n = getwrap(r,c-1) + getwrap(r,c+1)
			+ getwrap(r-1,c-1) + getwrap(r-1,c) + getwrap(r-1,c+1)
			+ getwrap(r+1,c-1) + getwrap(r+1,c) + getwrap(r+1,c+1);
		newgrid[r*numcols+c] = ( n==3 or ( n==2 and getwrap(r,c)==1 ) );
	}
   // (numrows-1,c)
	for (int c=0; c<numcols; ++c) {
		const int r=numrows-1;
		int n = getwrap(r,c-1) + getwrap(r,c+1)
			+ getwrap(r-1,c-1) + getwrap(r-1,c) + getwrap(r-1,c+1)
			+ getwrap(r+1,c-1) + getwrap(r+1,c) + getwrap(r+1,c+1);
		newgrid[r*numcols+c] = ( n==3 or ( n==2 and getwrap(r,c)==1 ) );
	}
	// (r,0)
	for (int r=1; r<numrows-1; ++r) {
		constexpr int c=0;
		int n = getwrap(r,c-1) + getwrap(r,c+1)
			+ getwrap(r-1,c-1) + getwrap(r-1,c) + getwrap(r-1,c+1)
			+ getwrap(r+1,c-1) + getwrap(r+1,c) + getwrap(r+1,c+1);
		newgrid[r*numcols+c] = ( n==3 or ( n==2 and getwrap(r,c)==1 ) );
	}
	// (r,numcols-1)
	for (int r=1; r<numrows-1; ++r) {
		const int c = numcols-1;
		int n = getwrap(r,c-1) + getwrap(r,c+1)
			+ getwrap(r-1,c-1) + getwrap(r-1,c) + getwrap(r-1,c+1)
			+ getwrap(r+1,c-1) + getwrap(r+1,c) + getwrap(r+1,c+1);
		newgrid[r*numcols+c] = ( n==3 or ( n==2 and getwrap(r,c)==1 ) );
	}
	// Interior
	auto nit = newgrid.begin();
	nit += numcols;
	auto prev_row = grid.begin();
	auto curr_row = prev_row + numcols;
	auto next_row = curr_row + numcols;
	for (int r=1; r<numrows-1; ++r) {
		++nit;
		for (int c=1; c<numcols-1; ++c) {
			int n = prev_row[0] + prev_row[1] + prev_row[2];
			n += curr_row[0] + curr_row[2];
			n += next_row[0] + next_row[1] + next_row[2];
			*nit = ( n==3 or ( n==2 and curr_row[1]==1 ) );
			++nit; ++prev_row; ++curr_row; ++next_row;
		}
		++nit;
		++prev_row; ++curr_row; ++next_row;
		++prev_row; ++curr_row; ++next_row;
	}
	std::swap(grid,newgrid);
}

using gamelife = game_of_life<int>;

double do_test(int seed, int size, int iters)
{
	myrand rng{seed};
	gamelife gol{size,size};
	gol.init_random(rng);
	// Now time
	std::chrono::steady_clock::time_point t1,t2;
	std::chrono::duration<double> time_span;
	t1 = std::chrono::steady_clock::now();
	for (int i=0; i<iters; ++i) { gol.iterate(); }
	t2 = std::chrono::steady_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	return time_span.count();
}

int main()
{
	myrand rng{12};
	for (int i=0; i<20; ++i) {
		std::cout << rng.get() << std::endl;
	}

	gamelife gol{10,10};
	gol.init_random(rng);
	std::cout << gol;
	gol.iterate();
	std::cout << gol;

	double times[3];
	for (auto &t : times) { t = do_test(12,2000,4); }
	std::cout << "Times taken:" << std::setprecision(8);
	for (auto t : times) { std::cout << t << ", "; }
	std::cout << std::endl;

	return 0;
}
