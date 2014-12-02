//mpicc -o E parrellel\(distributed\)pi.c && mpirun -np 2 E 1000
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

float get_pi(int N){
  //Monte Carlo method of calculating pi the argument is the number of samples to use.
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

int main(int argc,char *argv[]){
  int N,cores,id,i;
  float pi,pi0;
  MPI_Status status;
  MPI_Init(&argc, &argv);//initialise mpi world
  MPI_Comm_rank(MPI_COMM_WORLD,&id);//get process id for all processes
  srand(id+time(NULL));//seed random number generator to be different for all processes
   if(id==0){//for the root process only
     sscanf(argv[1],"%d",&N);//get number of samples and number of cores to use
     MPI_Comm_size(MPI_COMM_WORLD,&cores);
     for(i=1;i<cores;++i){//broadcast the information to the other cores
       MPI_Send(&N,1,MPI_INT,i,1,MPI_COMM_WORLD);
       MPI_Send(&cores,1,MPI_INT,i,1,MPI_COMM_WORLD);
     }
   }
   else{//for all other cores
     MPI_Recv(&N,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);//receive information from the root process
     MPI_Recv(&cores,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
   }
   pi=get_pi(N/cores);//call the function and each calculate pi
   if(id==0){//for the root process only
     for(i=1;i<cores;++i){//get the answers from all the other processes and add them the its own
      MPI_Recv(&pi0,1,MPI_FLOAT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);//loops through all ids but will always take the next available answer to save time (incase 1 is last to finish)
       pi+=pi0;
     }
     printf("%f\n",pi/cores);//print average
   }
   else{//for all other processes
     MPI_Send(&pi,1,MPI_FLOAT,0,1,MPI_COMM_WORLD);//when finished send the answer to the root process
   }
  MPI_Finalize();//close all child processes before exit
  return 0;
}
