#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "time.h"

#include<cblas.h>

extern void sgemm_(char*, char*, int*, int*,int*, float*, float*,
		   int*, float*, int*, float*, float*, int*);

int main(int argc, char* argv[])
{
  int i;
  printf("test!\n");
  if(argc<5){
    printf("Input Error\n");
    return 1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int k = atoi(argv[3]);
  int num_threads = atoi(argv[4]);
  int sizeofa = m * k;
  int sizeofb = k * n;
  int sizeofc = m * n;
  char ta = 'N';
  char tb = 'N';
  float alpha = 1.0;
  float beta = 0.00;

  struct timeval start,finish;
  double duration;

  float* A = (float*)malloc(sizeof(float) * sizeofa);
  float* B = (float*)malloc(sizeof(float) * sizeofb);
  float* C = (float*)malloc(sizeof(float) * sizeofc);

  srand((unsigned)time(NULL));

  for (i=0; i<sizeofa; i++)
    A[i] = i%3+1;//(rand()%100)/10.0;

  for (i=0; i<sizeofb; i++)
    B[i] = i%3+1;//(rand()%100)/10.0;

  for (i=0; i<sizeofc; i++)
    C[i] = i%3+1;//(rand()%100)/10.0;

  //set number of threads
  openblas_set_num_threads(num_threads);

  int itermax = 20;
  
  //#if 0
  printf("m=%d,n=%d,k=%d,alpha=%lf,beta=%lf,sizeofc=%d\n",m,n,k,alpha,beta,sizeofc);
  gettimeofday(&start, NULL);
  for(int i=0; i<itermax; ++i){
  sgemm_(&ta, &tb, &m, &n, &k, &alpha, A, &m, B, &k, &beta, C, &m);
  }
  gettimeofday(&finish, NULL);

  duration = ((double)(finish.tv_sec-start.tv_sec)*1000000
	       + (double)(finish.tv_usec-start.tv_usec)) / 1000000;
  double gflops = (double)itermax*(2.0 * m *n*k);
  gflops = gflops/duration*1.0e-9;

  FILE *fp;
  fp = fopen("timeSGEMM.txt", "a");
  fprintf(fp, "%dx%dx%d\t%lf s\t%.4f \t%d\n", m, n, k, duration, gflops, num_threads);
  fclose(fp);

  free(A);
  free(B);
  free(C);
  return 0;
}
