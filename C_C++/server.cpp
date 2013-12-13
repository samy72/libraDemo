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

//  {"-ns", "-a", "xxx.xxx.xxx.xxx", "-p", "yyyyy"} 
int main(int argc, char** argv)
{

	if (libra_Init(argc, argv) != 0) return 1;
	
	while(1);

	libra_Shutdown();

	return 0;
}

#endif
