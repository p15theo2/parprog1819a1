#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

//get_walltime - Function used to measure the duration of an execution
void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double) (tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(int argc, char* argv[]){
       //Definitions - helptab table is used to help us calculate every element of c table
	float *atab,*btab,*ctab,*helptab;

  int a = posix_memalign((void**)&atab, 16, N*N*sizeof(float));
  if (a!=0){ printf("allocation error!\n"); 
  return 1;
  }

   int b = posix_memalign((void**)&btab, 16, N*N*sizeof(float));
  if (b!=0) { 
    printf("allocation error\n");
    free(atab);
    return 1;
   }
//multiplications results table
  int c = posix_memalign((void**)&ctab, 16, N*N*sizeof(float));
  if (c!=0) { 
    printf("allocation error!\n");
    free(atab);
    free(btab);
    return 1;
  }
//help table
   int help = posix_memalign((void**)&helptab, 16, 4*sizeof(float));
    if(help != 0) {
        free(atab);
        free(btab);
        free(ctab);
        return 1;
    }
	
//initialization 
  for (int i=0;i<N*N;i++) {
     atab[i] = 2.0;
     btab[i] = 3.0;
     ctab[i] = 0.0;
  }
//initialization of the help table 
  for(int i=0; i<4; i++) {
   helptab[i] = 0.0;
  }
//Definitiom of parameters used in get_walltime function
 double ts=0,te=0;
//Definition of pointers(to avoid segmentation errors)
float *pc=ctab ,*phelp=helptab;
//__m127
__m128 *va = (__m128*)atab, *vb=(__m128*)btab, *vc = (__m128*)ctab, *vhelp = (__m128*)helptab;
//using get_walltime to measure execution time ""start""
get_walltime(&ts);
  for (int k=0;k<N;k++) {
    vb=(__m128*)btab;
    for (int j=0;j<N;j++) {
      va=(__m128*)(atab+(k*N));
      *vhelp = _mm_set_ps(0.0,0.0,0.0,0.0);
      for(int i=0; i<N; i+=4){
      *vhelp = _mm_add_ps(*vhelp, _mm_mul_ps(*va, *vb));
      va++; vb++;
      }

      phelp = helptab;
      for(int i=0; i<4; i++) {
	*pc = *pc + *phelp;
	phelp++;
	}
       pc++;
    }
  }

//calling get_walltime to end measuring ""end""
get_walltime(&te);
 
 for (int i=0; i<N*N; i++) {
        if (ctab[i]!= 2.0 * 3.0 * N) {
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
  free(atab);
  free(btab);
  free(ctab);
  free(helptab);
 

  return 0;
}

//Program compliles using the following:
//gcc -Wall -O2 EX1.c -o Ex1 -DN=400
