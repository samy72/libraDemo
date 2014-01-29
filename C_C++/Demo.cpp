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


results computeBasic(){
  int i, j, k, warm;
  float A[M_SIZE][M_SIZE], B[M_SIZE][M_SIZE], C[M_SIZE][M_SIZE], s;
  long long flopCount;
  double startTime, time, gflops;
  results res;

  printf("\n*********** Basic ***********\n");

  memset(&res, 0, sizeof(results));
  
  // float 1.0 initialization
  
  for (  i = 0 ; i < M_SIZE ; i++ )
    for ( j = 0 ; j < M_SIZE ; j++ )
      A[i][j] = 1.0;
  
  for ( i = 0 ; i < M_SIZE ; i++ )
    for ( j = 0 ; j < M_SIZE ; j++ )
      B[i][j] = 1.0;
  
  // 4 Warmup the cache ?
  for ( i = 0 ; i < M_SIZE ; i++ ){
    for ( j = 0 ; j < M_SIZE ; j++ ){
		C[i][j] = 0.0;
		for ( k = 0 ; k < M_SIZE ; k++ ) C[i][j] += A[i][k]*B[k][j];
    }
  }
  
  // Measure
  startTime = libra_GetTime();
  
  for ( i = 0 ; i < M_SIZE ; i++ ){
    for ( j = 0 ; j < M_SIZE ; j++ ){
		C[i][j] = 0.0;
		for ( k = 0 ; k < M_SIZE ; k++ ) C[i][j] += A[i][k]*B[k][j];
    }
  }
  //sleep(1);
  time = libra_GetTime() - startTime;
  /* print result
    for ( i = 0 ; i < M_SIZE ; i++ ){
		for ( j = 0 ; j < M_SIZE ; j++ ){
			printf("%g ", C[i][j]);
		}
		printf("\n");
	}
	*/
  printf("Total time : %g%s", time*1000, " milliseconds\n");

  /* Compute a performance measurement. Gigaflop / s. */
  flopCount = (2*M_SIZE-1)*M_SIZE*(long long)M_SIZE;
  gflops = flopCount / time / 1e9;
  printf("%g%s", gflops, " GFlop/s.\n");

  res.duration = time;
  return res;
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
	return 1;
}

int computeCPU()
{
	printf("\n********** CPU 1 **********\n");
	if(!libra_SetCurrentDevice(CPU_DEVICE_1)){
		printf("\nCPU 1 Not Available\n");
		return 0;
	}
	printf("\nCPU Backend SET\n");
	compute();
	return 1;
}

int computeGPU()
{
	printf("\n********** GPU 1 **********\n");
	if(!libra_SetCurrentDevice(GPU_DEVICE_1)){
		printf("\nGPU 1 Not Available\n");
		return 0;
	}

	if(libra_SetCurrentBackend(CUDA_BACKEND)){
		printf("\nCUDA Backend SET\n");
		compute();
	}
	else{
		printf("\nCUDA Backend cannot be set\n");
	}

	if(libra_SetCurrentBackend(OPENCL_BACKEND)){
		printf("\nOpenCL Backend SET\n");
		compute();
	}
	else{
		printf("\nOpenCL Backend cannot be set\n");
	}

	if(libra_SetCurrentBackend(OPENGL_BACKEND)){
		printf("\nOpenGL Backend SET\n");
		compute();
	}
	else{
		printf("\nOpenGL Backend cannot be set\n");
	}

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
	TypeOfTest tot = ALL;

	if (libra_Init(argc, argv) != 0) return 1;

	libra_SetDefaultDataType(GFLOAT32);

	printf("SGEMM: Single-precision General Matrix Multiply\n");
	printf("Compute MxM with a Square MATRIX SIZE = %d\n", M_SIZE);
	printf("Same code but many ways:\n\n");
	printf("- Basic C language computation: the for(i, j, k) loop\n");
	printf("- CPU device Backend\n");
	printf("- GPU / CUDA Backend\n");
	printf("- GPU / OpenCL Backend\n");
	printf("- GPU / OpenGL Backend\n");
	printf("- Cloud Computing on local node. Ex: -nc, -a, 127.0.0.1, -p, 12345, -b, CPU_BACKEND, -remoteCompute\n");
	printf("\nLet's Start...\n");
	
	if((ALL==tot) || (NONE==tot)) computeBasic();
	if((ALL==tot) || (CPU==tot)) computeCPU();
	if((ALL==tot) || (GPU==tot)) computeGPU();
	if((ALL==tot) || (CLOUD==tot)) computeCLOUD(argc, argv);

	libra_Shutdown();

	return 0;
}

#endif
