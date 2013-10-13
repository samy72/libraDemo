#if 1
#include <stdio.h>
#include <string.h>
#include <iostream>
// Libra
#include <Libra.h>    // C and C++ API

#ifdef _WIN32>
#include <windows.h>
#else
#include <unistd.h>
#endif

#define N_SAMPLES 10

#ifdef _WIN32
#define M_SIZE 256
#elif
#define M_SIZE 1024
#endif

#define FLOPCOUNT ((2*M_SIZE-1)*M_SIZE*(long long)M_SIZE)

typedef enum {
	ALL,
	NONE,
	CPU,
	GPU,
	CLOUD,
} TypeOfTest;

typedef struct results {
  double duration;
  double avg;
  double sigma2;
} results;


results computeNoAcc(){
  int i, j, k;
  float A[M_SIZE][M_SIZE], B[M_SIZE][M_SIZE], C[M_SIZE][M_SIZE], s=0;
  double startTime, time, duration;

  results res;
  memset(&res, 0, sizeof(results));

  // float 1.0 initialization

  for (  i = 0 ; i < M_SIZE ; i++ )
    for ( j = 0 ; j < M_SIZE ; j++ )
      A[i][j] =1.0;
  
  for ( i = 0 ; i < M_SIZE ; i++ )
    for ( j = 0 ; j < M_SIZE ; j++ )
      B[i][j] = 1.0;

  	startTime = libra_GetTime();

  for ( i = 0 ; i < M_SIZE ; i++ )
    for ( j = 0 ; j < M_SIZE ; j++ )
      {
	for ( k = 0 ; k < M_SIZE ; k++ ) s = s + A[i][k]*B[k][j];
        C[i][j] = s;
        s = 0.0;
      }

  //  sleep(1);

	res.duration = libra_GetTime() - startTime;
	//printf("Total time : %g%s", res.duration*1000, " milliseconds\n");
  return res;
}

int computeNoAccLoop(){
  results res[N_SAMPLES];
  double avg = 0;
  double gflops = 0;

  printf("\n");
  printf("*******************************************\n\n");
  printf("SGEMM : WITHOUT LIBRA matrix multiplication\n\n");
  printf("*******************************************\n\n");


  for(int i=0; i<N_SAMPLES; i++) {
    res[i] = computeNoAcc();
    avg = (avg*i + res[i].duration) / (i+1);
    //printf("Total time : %g%s", res[i].duration*1000, " milliseconds\n");
    if (N_SAMPLES-1 == i){
		printf("Average Total time : %g%s", avg*1000, " milliseconds\n");
		if (0!=avg){
			gflops = FLOPCOUNT / avg / 1e9;
			printf("%g%s", gflops, " GFlop/s.\n");
		}
		else{
			printf("Unknown GFlop/s.\n");
		}
	}
  }
  return 0;
}

int compute()
{
	long long flopCount;
	double startTime, time, gflops;

	gVar A, B, C;

	/* Create two N by N matrices. */
	A = ones(M_SIZE);
	B = A;

	/* warm up phase of the gpu! (to get accurate timing results) */
	C = A * B; C = A * B;
	C = A * B; C = A * B; 

	startTime = libra_GetTime();

	/* Perform matrix multiplication */
	C = A * B;
	
	time = libra_GetTime() - startTime;
	printf("Total time : %g%s", time*1000, " milliseconds\n");

	/* Compute a performance measurement. Gigaflop / s. */
	flopCount = (2*M_SIZE-1)*M_SIZE*(long long)M_SIZE;
	gflops = flopCount / time / 1e9;
	printf("%g%s", gflops, " GFlop/s.\n");
	return 0;
}

int computeCPU()
{
	printf("\n");
	printf("*********************************\n\n");
	printf("SGEMM : CPU matrix multiplication\n\n");
	printf("*********************************\n\n");

	libra_SetCurrentDevice(CPU_DEVICE_1);
	compute();
	return 0;
}

int computeGPU()
{
	printf("\n");
	printf("*********************************\n\n");
	printf("SGEMM : GPU matrix multiplication\n\n");
	printf("*********************************\n\n");

	libra_SetCurrentDevice(GPU_DEVICE_1);
	compute();
	return 0;
}

int computeCLOUD()
{
  printf("TODO\n");
  return 0;
}

int main(int argc, char** argv)
{
	TypeOfTest tot = ALL;

	if (2 == argc){
	  if (!strcmp(argv[1], "CPU")) tot = CPU;
	  else if (!strcmp(argv[1], "NONE")) tot = NONE;
	  else if (!strcmp(argv[1], "GPU")) tot = GPU;
	  else if (!strcmp(argv[2], "CLOUD")) tot = CLOUD;
	}

	if (libra_Init(argc, argv) != 0)
		return 1;

	libra_SetDefaultDataType(GFLOAT32);

	printf("MATRIX SIZE = %d\n", M_SIZE);
	
	if((ALL==tot) || (NONE==tot)) computeNoAccLoop();
	if((ALL==tot) || (CPU==tot)) computeCPU();
	if((ALL==tot) || (GPU==tot)) computeGPU();
	//if((ALL==tot) || (CLOUD==tot)) computeCLOUD();

	libra_Shutdown();

	return 0;
}

#endif
