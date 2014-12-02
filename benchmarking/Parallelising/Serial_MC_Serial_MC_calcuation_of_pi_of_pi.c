#include <stdio.h>
#include <stdlib.h>

float get_pi(int N){
  //Monte Carlo method of calculating pi the argument is the number of samples to use.
  int pi=0,n=0;
  float x,y;
  while(n<N){
    x=((float)rand())/((float)RAND_MAX);
    y=((float)rand())/((float)RAND_MAX);
    ++n;
    if(x*x+y*y<=1.0){
      ++pi;
    }
  }
  return 4.0*((float)pi)/((float)n);
}

int main(int argc,char *argv[]){
  int N;
  sscanf(argv[1],"%d",&N);
  printf("%f\n",get_pi(N));
  return 0;
}
