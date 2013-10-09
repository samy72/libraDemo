#if 1
#include <stdio.h>
#include <string.h>
#include <iostream>
// Libra
#include <Libra.h>    // C and C++ API

typedef enum {
	ALL,
	NONE,
	CPU,
	GPU,
	CLOUD,
} TypeOfTest;

int compute()
{
	const int N = 1024;
	long long flopCount;
	double startTime, time, gflops;

	gVar A, B, C;

	/* Create two N by N matrices. */
	A = ones(N);
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
	flopCount = (2*N-1)*N*(long long)N;
	gflops = flopCount / time / 1e9;
	printf("%g%s", gflops, " GFlop/s.\n");
}

int computeCPU()
{
	libra_SetCurrentDevice(CPU_DEVICE_1);
	printf("CPU Computation\n");
	compute();
	return 0;
}

int computeGPU()
{
	libra_SetCurrentDevice(GPU_DEVICE_1);
	printf("GPU Computation\n");
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

	printf("*****************************\n\n");
	printf("SGEMM : matrix multiplication\n\n");
	printf("*****************************\n\n");
	
	if (2 == argc){
	  if (!strcmp(argv[1], "CPU")) tot = CPU;
	  else if (!strcmp(argv[1], "GPU")) tot = GPU;
	  else if (!strcmp(argv[2], "CLOUD")) tot = CLOUD;
	}

	if (libra_Init(argc, argv) != 0)
		return 1;

	libra_SetDefaultDataType(GFLOAT32);

	if((ALL==tot) || (CPU==tot)) computeCPU();
	if((ALL==tot) || (GPU==tot)) computeGPU();
	if((ALL==tot) || (CLOUD==tot)) computeCLOUD();

	libra_Shutdown();

	return 0;
}

#endif
