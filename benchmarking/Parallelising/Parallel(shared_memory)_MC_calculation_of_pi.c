//gcc -fopenmp -Wall -o E parrellel\(shared\)pi.c && ./E 1000 2
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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

int main(int argc,char *argv[]){
  int N,cores;
  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&cores);
#pragma omp parallel
{
  omp_set_num_threads(cores);//set the number of threads
}
  printf("%f\n",get_pi(N));
  return 0;
}
