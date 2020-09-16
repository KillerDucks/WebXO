# Compiler Variables
Compiler = /usr/bin/g++

Flags = -Wall -pthread -std=c++14 -O2

# Program Variables
ProgName = WebXO_TestSuite

# Build Directory Variables
Build := ./Tests/build
# Program Executable Variables
runProgram := ./$(Build)/$(ProgName)

# File Variables
Source_Files :=  $(shell find ./Tests/ -type f -name '*.cpp')

build: clean ${Build}/$(ProgName)

${Build}/$(ProgName): $(Source_Files)
	$(Compiler) $(Flags) $^ -lgtest_main -lgtest -lpthread -lWebX -lz -o ${Build}/$(ProgName)

clean: 
	@echo "Cleaning Build System..."
	@rm -rf ./build/*
	@echo "!!! Done Cleaning the TEST System !!!"

test:
	$(runProgram)

gdb:
	$(gdb runProgram)