# Compiler Variables
Compiler = /usr/bin/g++
Flags = -Wall -g -std=c++14 -O0

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

# $(Objects)/pMain.o $(Objects)/WebX_Sockets_Obj.o

# Make the Debug Build
$(Build_Debug)/$(ProgName): $(Objects)/pMain.o WebXLib
	$(Compiler) $(Flags) -o $(Build_Debug)/$(ProgName) $^ -lstdc++fs
	
# WebXLib
rad: $(Object_Files)
	$(Compiler) $(Flags) -o $(Build_Debug)/$(ProgName) $^ -lstdc++fs


$(Build)/$(ProgName): $(Objects)/libWebX.so.1.0 $(Objects)/pMain.o
	$(Compiler) $(Flags) -o $(Build_Debug)/$(ProgName) $^ -L/usr/local/lib -lWebX -lpthread

# Main Exec Object
$(Objects)/pMain.o: $(Source)/pMain.cpp
	$(Compiler) $(Flags) -c $^ -o $(Objects)/pMain.o


LibWebX: $(Objects)/libWebX.so.1.0 moveLib

moveLib:
	$(shell  cp build/obj/libWebX.so.1.0 /usr/local/lib)
	$(shell  ln -sf /usr/local/lib/libWebX.so.1.0 /usr/local/lib/libWebX.so.1)
	$(shell  ln -sf /usr/local/lib/libWebX.so.1.0 /usr/local/lib/libWebX.so)

# Make the object files
$(Objects)/libWebX.so.1.0: $(Objects)/Logarithm.o $(Objects)/HTTP.o $(Objects)/Sockets.o $(Objects)/Directory.o 
	$(Compiler) $(Flags) -shared -Wl,-soname,libWebX.so.1 $^ -o $@ -lstdc++fs

$(Objects)/Logarithm.o: $(WebXLib)/Logarithm.cpp	
	$(Compiler) $(Flags) -c -fPIC $^ -o $@

$(Objects)/HTTP.o: $(WebXLib)/HTTP.cpp	
	$(Compiler) $(Flags) -c -fPIC $^ -o $@

$(Objects)/Sockets.o: $(WebXLib)/Sockets.cpp
	$(Compiler) $(Flags) -c -fPIC $^ -o $@ -lpthread

$(Objects)/Directory.o: $(WebXLib)/Directory.cpp
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