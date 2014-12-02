//gcc -fopenmp -Wall -o E parrellel\(shared\)pi.c && ./E 1000 2

// Original author Craig Hall
// 2 Dec 14: Matt Daws added timing code and a single threaded test function
//           and added random number seed setting.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include <sys/time.h>

float get_pi_single(int N)
{
  int pi=0,n;
  float x,y;
  for(n=0;n<N;++n){
    x=((float)rand())/((float)RAND_MAX);
    y=((float)rand())/((float)RAND_MAX);
    if(x*x+y*y<=1.0){
      ++pi;
    }
  }
  return 4.0*((float)pi)/((float)N);
}

float get_pi(int N){
  //Monte Carlo method of calculating pi the argument is the number of samples to use.
  int pi=0,n;
  float x,y;
#pragma omp parallel private(n,x,y)reduction(+:pi)//enter parallel region of code
{
#pragma omp for schedule(dynamic)//collapse loop (sharing work between cores. In dynamic mode the task will go to the next free core.) 
  for(n=0;n<N;++n){
    x=((float)rand())/((float)RAND_MAX);
    y=((float)rand())/((float)RAND_MAX);
    if(x*x+y*y<=1.0){
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
#pragma omp parallel
{
  omp_set_num_threads(cores);//set the number of threads
}
  struct timeval t1, t2;
  gettimeofday(&t1,0);
  srand(12345);
  printf("%f\n",get_pi(N));
  gettimeofday(&t2,0);
  printf("Start time: %ld secs, %ld msecs.\n", t1.tv_sec, t1.tv_usec);
  printf("End time: %ld secs, %ld msecs.\n", t2.tv_sec, t2.tv_usec);
  double dt1 = t1.tv_sec + ((double)t1.tv_usec)/1000000.0;
  double dt2 = t2.tv_sec + ((double)t2.tv_usec)/1000000.0;
  printf("Elapsed time: %f.\n", dt2-dt1);

  gettimeofday(&t1,0);
  srand(12345);
  printf("%f\n",get_pi_single(N));
  gettimeofday(&t2,0);
  dt1 = t1.tv_sec + ((double)t1.tv_usec)/1000000.0;
  dt2 = t2.tv_sec + ((double)t2.tv_usec)/1000000.0;
  printf("Single Thread time: %f.\n", dt2-dt1);

  gettimeofday(&t1,0);
  srand(12345);
  printf("%f\n",get_pi_single(N));
  gettimeofday(&t2,0);
  dt1 = t1.tv_sec + ((double)t1.tv_usec)/1000000.0;
  dt2 = t2.tv_sec + ((double)t2.tv_usec)/1000000.0;
  printf("Single Thread time: %f.\n", dt2-dt1);

  return 0;
}
