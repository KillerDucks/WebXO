# WebXO ![AWS CodeBuild](https://codebuild.us-east-1.amazonaws.com/badges?uuid=eyJlbmNyeXB0ZWREYXRhIjoiUmt5Wmd0ME94YkVYWkovVyt1UzB2bjVDQ2RXUmVJK2xxYW5vbjJGZmQ0WkpCbHoxTlhFSnRIbjlGb1BhcVZBWGNLRHQvdG04cjBQcFkvT1NHckZ5c0wwPSIsIml2UGFyYW1ldGVyU3BlYyI6ImZGcTZHUjNUMG9Kakt3VVEiLCJtYXRlcmlhbFNldFNlcmlhbCI6MX0%3D&branch=master) [![Build Status](https://dev.azure.com/WebXO/WebXO/_apis/build/status/KillerDucks.WebXO?branchName=master)](https://dev.azure.com/WebXO/WebXO/_build/latest?definitionId=1&branchName=master)
A super simple *`but poorly written`* web server written in C++. 

``Complete Package (Exec + Library) @ 616K``
> [v1.2.0] Codename VirtualizeEverything

# Demo
> This demo will take some time to start as I am using the free tier app services in Azure, however a smaller image is in the works to be used specifically for demonstration purposes

https://webxo.azurewebsites.net

# Installation
> Note: Currently everything is built with debug support this will be addressed in a future Makefile update, currently you will have to manually change the flags in the Makefile

The server can be installed locally (not recommended) or in a docker container (highly recommended).

## Install Script
This is not advised for this build of WebXO as the CMake components have not been fully tested thus it may have undesired effects on your system. Docker is the only way to install this build of WebXO 

## Github (Prebuilt Executable + Library)
This method is best if you don't want to build the server yourself and you don't have docker to deploy the server. You will need to place the Lib into a path that is on the LDD search list or you can follow these steps (similar to the install script)

```sh
# Copy the Lib
$> cp /<Path2Lib>/libWebX.so.1.0 /usr/local/lib

# Create the needed symbolic links
$> ln -sf /usr/local/lib/libWebX.so.1.0 /usr/local/lib/libWebX.so.1
$> ln -sf /usr/local/lib/libWebX.so.1.0 /usr/local/lib/libWebX.so

# Export the LD Path
$> export LD_LIBRARY_PATH=/usr/local/lib

# Run the Server
$> ./<Path2Server>/WebXServer --help
```

## Docker (via Docker Hub)
This method is highly recommended and is therefore the preferred way of using the server.

To get the server up and running run the following commands.

```sh
# Pull the docker image from Docker Hub
$> docker pull killerducks/webxo-server:latest

# This will run the test page
$> docker run -d -p 8080:8080 killerducks/webxo-server:latest

# This way will allow you to attach your http folder to the server
$> docker run -d -v <localfilepath>:/var/webxo/www -p 8080:8080 killerducks/webxo-server:latest --basepath="/var/webxo/www"
```

## Docker (via self build)
This method is a better choice over the self make, however using the pre-made docker image is still the recommended choice.

The shell script options are simple
```sh
# Make the full build using the Ubuntu docker image as the base
$> ./makeDocker_env.sh -f
# Make the tiny build using the Alpine docker image as the base
$> ./makeDocker_env.sh -t
# Make the executable only image using the Alpine docker image as the base
$> ./makeDocker_env.sh -e
```

> Note: The '-e' option requires you to build the binaries locally via make as shown in [Install Script](#Install-Script) and then build the docker image.