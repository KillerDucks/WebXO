# Compiler Variables
Compiler = /usr/bin/g++
Flags = -Wall -g -std=c++11

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
debug: $(Build_Debug)/$(ProgName)

# $(Objects)/pMain.o $(Objects)/WebX_Sockets_Obj.o

# Make the Debug Build
$(Build_Debug)/$(ProgName): $(Objects)/pMain.o WebXLib
	$(Compiler) $(Flags) -o $(Build_Debug)/$(ProgName) $^ -lstdc++fs
	
# Main Exec Object
$(Objects)/pMain.o: $(Source)/pMain.cpp
	$(Compiler) $(Flags) -c $^ -o $(Objects)/pMain.o

# WebXLib
all: $(Object_Files)
	$(Compiler) $(Flags) -o $(Build_Debug)/$(ProgName) $^ -lstdc++fs



# Make the object files
$(Objects)/WebX_Sockets_Obj.o: $(WebXLib)/Sockets.cpp
	$(Compiler) $(Flags) -c $^ -o $(Objects)/WebX_Sockets_Obj.o

$(Objects)/WebX_Directory_Obj.o: $(WebXLib)/Directory.cpp
	$(Compiler) $(Flags) -c $^ -o $(Objects)/WebX_Directory_Obj.o


clean: 
	@echo "Cleaning Build System..."
	@rm -rf ./build/obj/*
	@rm -rf ./build/debug/*
	@rm -rf ./build/release/*
	@echo "!!! Done Cleaning the Build System !!!"

exec:
	$(runProgram)