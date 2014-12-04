//gcc -fopenmp -Wall -o E parrellel\(shared\)pi.c && ./E 1000 2

// Original author Craig Hall
// 2 Dec 14: Matt Daws added timing code and a single threaded test function
//           and added random number seed setting.
//   omp_set_num_threads should not be in #prgama block.
// 4 Dec 14: Matt Daws changed rand() to thread-safe version.
//   Doesn't seem to help in making the multi-threaded version competative in speed!

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include <sys/time.h>

// Mingw-64 at least does not implement this function
// This implements a 32-bit linear congruence generator; this is not a good
// type of PRND, but will suffice as proof of concept.
struct drand48_data {
	unsigned int X;
};
int drand48_r(struct drand48_data *buf, double *d)
{
	unsigned long long Y = 1103515245;
	Y = Y * ((unsigned long long)buf->X) + 12345;
	buf->X = (Y & 2147483647);
	*d = ((double)buf->X) / 2147483647.0;
	return 0;
}
int erand48_r(unsigned short int xsubi[3], struct drand48_data *buf, double *d)
{
	buf->X = xsubi[0];
	buf->X += ((unsigned int)xsubi[1]) << 16;
	return drand48_r(buf, d);
}

float get_pi_single(int N)
{
  int pi=0,n;
  double x,y;
  struct drand48_data drand_buf;
  unsigned short int seed[3] = {1,2,3};
  erand48_r(seed, &drand_buf, &x);
  for(n=0;n<N;++n){
    drand48_r(&drand_buf, &x);
    drand48_r(&drand_buf, &y);
    if (x*x+y*y<=1.0) { ++pi; }
  }
  return 4.0*((float)pi)/((float)N);
}

float get_pi(int N){
  //Monte Carlo method of calculating pi the argument is the number of samples to use.
  int pi=0,n;
  double x,y;
  struct drand48_data drand_buf;
  unsigned short int seed[3] = {1,2,3};
  erand48_r(seed, &drand_buf, &x);
#pragma omp parallel private(n,x,y,drand_buf)reduction(+:pi)//enter parallel region of code
{
#pragma omp for schedule(static)//collapse loop ('static' seems to work; 'dynamic' does not...)
  for(n=0;n<N;++n) {
    drand48_r(&drand_buf, &x);
    drand48_r(&drand_buf, &y);
    if(x*x+y*y<=1.0){
      #pragma omp atomic
      ++pi;
    }
  }
}
  return 4.0*((float)pi)/((float)N);
}

int main(int argc,char *argv[])
{
  if (argc<3) {
    printf("Usage: %s N cores.\n", argv[0]);
    return -1;
  }
  int N,cores;
  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&cores);
  omp_set_num_threads(cores);//set the number of threads
  struct timeval t1, t2;
  gettimeofday(&t1,0);
  printf("%f\n",get_pi(N));
  gettimeofday(&t2,0);
  printf("Start time: %ld secs, %ld msecs.\n", t1.tv_sec, t1.tv_usec);
  printf("End time: %ld secs, %ld msecs.\n", t2.tv_sec, t2.tv_usec);
  double dt1 = t1.tv_sec + ((double)t1.tv_usec)/1000000.0;
  double dt2 = t2.tv_sec + ((double)t2.tv_usec)/1000000.0;
  printf("Elapsed time: %f.\n", dt2-dt1);

  gettimeofday(&t1,0);
  printf("%f\n",get_pi_single(N));
  gettimeofday(&t2,0);
  dt1 = t1.tv_sec + ((double)t1.tv_usec)/1000000.0;
  dt2 = t2.tv_sec + ((double)t2.tv_usec)/1000000.0;
  printf("Single Thread time: %f.\n", dt2-dt1);

  gettimeofday(&t1,0);
  printf("%f\n",get_pi_single(N));
  gettimeofday(&t2,0);
  dt1 = t1.tv_sec + ((double)t1.tv_usec)/1000000.0;
  dt2 = t2.tv_sec + ((double)t2.tv_usec)/1000000.0;
  printf("Single Thread time: %f.\n", dt2-dt1);

  return 0;
}
