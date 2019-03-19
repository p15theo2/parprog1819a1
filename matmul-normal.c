#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//get_walltime - Function used to measure the duration of an execution
void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double) (tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(int argc, char* argv[]){
	//Definitions
	float *a, *b, *c;

  a = (float *)malloc(N*N*sizeof(float)); 
  if (a==NULL){ printf("allocation error!\n"); 
  return 1;
  }

  b = (float *)malloc(N*N*sizeof(float)); 
  if (b==NULL) { 
    printf("allocation error\n");
    free(a);
    return 1;
	}
//multiplications results table
  c = (float *)malloc(N*N*sizeof(float)); 
  if (c==NULL) { 
    printf("allocation error!\n");
    free(a);
    free(b);
    return 1;
  }
  
//initialization 
  for (int i=0;i<N*N;i++) {
     a[i] = 2.0;
     b[i] = 3.0;
     c[i] = 0.0;
  } 
//Definitions
 float sum;
//Definitiom of parameters used in get_walltime function
 double ts=0,te=0;
//Definition of pointers
float *pa,*pb,*pc;
//using get_walltime to measure execution time ""start""
get_walltime(&ts);
//seting up the pointers
pa = a;			
pb = b;			
pc = c;	
  
  for (int k=0;k<N;k++) {
    //column
    pb=b;
    for (int j=0;j<N;j++) {
      //row
      pa=a+(k*N);
      sum=0.0;
      for(int i=0; i<N; i++){
        sum = sum + (*pa) * (*pb);
        //next position
                pa++; 
                pb++; 
      }
      *pc = sum;
            pc++;
    }
  }

//calling get_walltime to end measuring ""end""
get_walltime(&te);
 
 for (int i=0; i<N*N; i++) {
        if (c[i]!= 2.0 * 3.0 * N) {
            printf("Error!\n");
            break;
        }
    }

//definitions
  double mflop;
//Calculating Mflops
  mflop = (N*N*N)/((te-ts)*1e6);
  
  printf("Execution Duration: %f,  Mflops/s: %f\n", te-ts, mflop);

//Deallocating
  free(b);
  free(a);
  free(c);
 

  return 0;
}

//Program compliles using the following:
//gcc -Wall -O2 EX1.c -o Ex1 -DN=400
