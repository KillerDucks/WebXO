#!/bin/bash

cd ./build

make

cd ..

# Run the server with the example site
./build/EXE/WebXOSrv --basepath="$(pwd)/ExampleSite" --port=8080