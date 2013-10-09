# compile C with /usr/bin/gcc
# compile CXX with /usr/bin/c++
C_FLAGS = -O3 -DNDEBUG -I"./../../../include" -I/usr/include/GL -fPIC

C_DEFINES = 

CXX_FLAGS = -O3 -DNDEBUG -I"./../../../include" -I/usr/include/GL -fPIC

CXX_COMMAND = /usr/bin/g++ -m32 -c
CXX_COMMAND_X64 = /usr/bin/g++ -m64 -c

CXX_DEFINES = 

LINKER_INCLUDES = -L"/usr/lib/" -L"./../../../lib/" -L"./../../../bin/"
LINKER_INCLUDES_X64 = -L"/usr/lib/" -L"./../../../lib/" -L"./../../../bin/x64/"

LINKER_FLAGS = -m32 -O3 -DNDEBUG -lLibra -lcufft -lcudart -lGLEW -glut -lamdocl32
LINKER_FLAGS_X64 = -m64 -O3 -DNDEBUG -lLibra_x64 -lcufft -lcudart -lGLEW -glut -lamdocl64

LINKER_COMMAND = /usr/bin/g++

PROGRAM = ./../../../bin/Demo
PROGRAM_X64 = ./../../../bin/x64/Demo_x64

#Detect arch
UNAME := $(shell uname -i)

$(PROGRAM) : Demo.cpp.o
	$(LINKER_COMMAND) $(LINKER_INCLUDES) -o $(PROGRAM) Demo.cpp.o $(LINKER_FLAGS) ./../../../lib/libLibraCpp.a
ifeq ($(UNAME), x86_64)
	$(LINKER_COMMAND) $(LINKER_INCLUDES_X64) -o $(PROGRAM_X64) Demo.cpp.x64.o $(LINKER_FLAGS_X64) ./../../../lib/libLibraCpp_x64.a
endif

CXX_OBJECTS = Demo.cpp.o Demo2.c.o
CXX_OBJECTS_X64 = Demo.cpp.x64.o Demo2.c.x64.o

Demo.cpp.o : Demo.cpp
	$(CXX_COMMAND) $(CXX_FLAGS) -o Demo.cpp.o Demo.cpp
ifeq ($(UNAME), x86_64)
	$(CXX_COMMAND_X64) $(CXX_FLAGS) -o Demo.cpp.x64.o Demo.cpp
endif

Demo2.c.o : Demo2.c
	$(CXX_COMMAND) $(CXX_FLAGS) -o Demo2.c.o Demo2.c
ifeq ($(UNAME), x86_64)
	$(CXX_COMMAND_X64) $(CXX_FLAGS) -o Demo2.c.x64.o Demo2.c
endif


RM = rm

clean:
	$(RM) -f $(CXX_OBJECTS) $(PROGRAM)
	$(RM) -f $(CXX_OBJECTS_X64) $(PROGRAM_X64)
