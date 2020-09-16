# Compiler Variables
Compiler = /usr/bin/g++

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    Flags = -Wall -g -std=c++14 -march=native -O2
else
    Flags = -Wall -std=c++14 -march=native -O2
endif

# This Makefile Flag removes the march optimisation and removes the debug flags
CLOUD ?= 0
ifeq ($(CLOUD), 1)
    Flags = -Wall -std=c++14 -O2
else
    Flags = -Wall -std=c++14 -march=native -O2
endif
# Flags = -Wall -g -std=c++14 -march=native -O2
# Flags = -Wall -g -std=c++14 -O2

# Program Variables
ProgName = WebXServer

# Lib Directory Variables
WebXLib := ./src/WebXLib
# Source Directory Variables
Source := ./src
Objects := build/obj
# Build Directory Variables
Build_Debug := ./build/debug
# Program Executable Variables
runProgram := ./$(Build_Debug)/$(ProgName)

# File Variables
Source_Files :=  $(shell find src/ -type f -name '*.cpp')
WebX_Files :=  $(shell find src/WebXLib -type f -name '*.cpp')
Object_Files := $(patsubst $(SRC_DIR)/%.cpp,$(Objects)/%.o,$(Source_Files))

# Debug
debug: $(Build)/$(ProgName)

$(Build)/$(ProgName): $(Objects)/libWebX.so.1.0 $(Objects)/pMain.o
	$(Compiler) $(Flags) -o $(Build_Debug)/$(ProgName) $^ -L/usr/local/lib -lWebX -lpthread -lz

# Main Exec Object
$(Objects)/pMain.o: $(Source)/pMain.cpp
	$(Compiler) $(Flags) -c $^ -o $(Objects)/pMain.o


LibWebX: $(Objects)/libWebX.so.1.0 moveLib

moveLib:
	$(shell  cp build/obj/libWebX.so.1.0 /usr/local/lib)
	$(shell  ln -sf /usr/local/lib/libWebX.so.1.0 /usr/local/lib/libWebX.so.1)
	$(shell  ln -sf /usr/local/lib/libWebX.so.1.0 /usr/local/lib/libWebX.so)

# Make the object files
$(Objects)/libWebX.so.1.0: $(Objects)/Logarithm.o $(Objects)/HTTP.o $(Objects)/Sockets.o $(Objects)/Directory.o  $(Objects)/Compression.o $(Objects)/Interception.o $(Objects)/VirtualHosts.o
	$(Compiler) $(Flags) -shared -Wl,-soname,libWebX.so.1 $^ -o $@ -lstdc++fs

$(Objects)/Logarithm.o: $(WebXLib)/Logarithm.cpp	
	$(Compiler) $(Flags) -c -fPIC $^ -o $@

$(Objects)/HTTP.o: $(WebXLib)/HTTP.cpp	
	$(Compiler) $(Flags) -c -fPIC $^ -o $@

$(Objects)/Sockets.o: $(WebXLib)/Sockets.cpp
	$(Compiler) $(Flags) -c -fPIC $^ -o $@ -lpthread

$(Objects)/Directory.o: $(WebXLib)/Directory.cpp
	$(Compiler) $(Flags) -c -fPIC $^ -o $@

$(Objects)/Compression.o: $(WebXLib)/Compression.cpp
	$(Compiler) $(Flags) -c -fPIC $^ -o $@ -lz

$(Objects)/Interception.o: $(WebXLib)/Interception.cpp
	$(Compiler) $(Flags) -c -fPIC $^ -o $@

$(Objects)/VirtualHosts.o: $(WebXLib)/VirtualHosts.cpp
	$(Compiler) $(Flags) -c -fPIC $^ -o $@


clean: 
	@echo "Cleaning Build System..."
	@rm -rf ./build/obj/*
	@rm -rf ./build/debug/*
	@rm -rf ./build/release/*
	@echo "!!! Done Cleaning the Build System !!!"

exec:
	$(runProgram)

gdb:
	$(gdb runProgram)