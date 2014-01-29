#if 1
#include <stdio.h>
#include <string.h>
#include <iostream>
// Libra
#include <Libra.h>    // C and C++ API

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const int M_SIZE=256;

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
	return 1;
}




int computeCLOUD(int argc, char **argv)
{
	printf("\n********** CLOUD **********\n");

	if (1==argc){
		if (!libra_SetCurrentComputeNode("192.168.1.4", 12345)){
			printf("\Cant connet to node, %s\n", libra_GetLastErrorMessage());
			printf("*********************\n");
			return 0;
		}
	}
	else if (libra_Init(argc, argv) != 0){
		printf("\nServer init Failed\n");
		printf("*********************\n");
		return 0;
	}
	compute();
	return 1;
}

int main(int argc, char** argv)
{
	if (libra_Init(argc, argv) != 0) return 1;

	libra_SetDefaultDataType(GFLOAT32);

	printf("SGEMM: Single-precision General Matrix Multiply\n");
	printf("Compute MxM with a Square MATRIX SIZE = %d\n", M_SIZE);
	printf("- Cloud Computing on local node. Ex: -nc, -a, 127.0.0.1, -p, 12345, -b, CPU_BACKEND, -remoteCompute\n");
	printf("\nLet's Start...\n");
	
	computeCLOUD(argc, argv);

	libra_Shutdown();

	return 0;
}

#endif
