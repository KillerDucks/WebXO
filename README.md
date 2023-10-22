# WebXO ![AWS CodeBuild](https://codebuild.us-east-1.amazonaws.com/badges?uuid=eyJlbmNyeXB0ZWREYXRhIjoiUmt5Wmd0ME94YkVYWkovVyt1UzB2bjVDQ2RXUmVJK2xxYW5vbjJGZmQ0WkpCbHoxTlhFSnRIbjlGb1BhcVZBWGNLRHQvdG04cjBQcFkvT1NHckZ5c0wwPSIsIml2UGFyYW1ldGVyU3BlYyI6ImZGcTZHUjNUMG9Kakt3VVEiLCJtYXRlcmlhbFNldFNlcmlhbCI6MX0%3D&branch=master) [![Build Status](https://dev.azure.com/WebXO/WebXO/_apis/build/status/KillerDucks.WebXO?branchName=master)](https://dev.azure.com/WebXO/WebXO/_build/latest?definitionId=1&branchName=master)
A super simple *`but poorly written`* web server written in C++. 

``Complete Package (Exec + Library) @ ~ 616K``

``AWS Badge is for Testing Builds / Azure Badge is for CI/CD``

> [v1.7.0] Codename SuperSimpleLayers (SSL)

# Demo
> This demo will take some time to start as I am using the free tier app services in Azure but should take approximately ``40 seconds``

https://webxo.azurewebsites.net

# Features
+ Serves Static Pages
    + Supported HTTP Methods [GET, HEAD, POST]
+ zlib deflate compression
+ Interception
    + Programatically intercept specific HTTP Requests and manipulate them
        - You will have to link your Server Initialisation code to use this feature [Interceptor](#Interceptor)
+ FolderViewer
    + Apache2 style directory listing
    + Placing a ``.FolderViewer`` file in a directory enables the feature

# Installation
> Note: Currently everything is built with debug support this will be addressed in a future CMake update, currently you will have to manually change the flags in the CMake

The server can be installed locally (not recommended) or in a docker container (highly recommended).

## CMake
This project relies on the CMake build system to be install on your system, it may be possible to update the ``Makefile.old`` in the project root directory if really necessary. If you are not using a IDE with a CMake addon you can run the following shell commands to setup CMake and then to use CMake to build the system.

```shell
# Sets up the build folder and CMake files 
mkdir ./build
cd ./build
cmake -G "Unix Makefiles" ../
cd ../

# Builds the Server
cmake --build ./build --config Debug --target all -- -j 6
```

## Github (Prebuilt Executable + Library) ~ OLD
> This is an old method using an old version of WebXO, its still usable but not recommend

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

# Examples

## Interceptor

> New: In the project root you can find the ``Examples`` directory, this folder contains full examples of how to use the interceptor. **The Examples are built by default**

The interception system is still in its early days but provides the basic support to manipulate HTTP Requests and serve the client something different to what they requested. 

This would be done for various reasons, you could add a layer of security to specific pages and use the interceptor to catch all requests and force the client to authenticate.

> To use this feature you will have to install libWebXO and then link your server initialization code to the library.

See ``src/pMain.cpp`` for an example of the **initialization code**, to intercept you will need an instance of ``WebXO::InterceptSettings`` as this will house your interception code and any other settings.

For example this code is how you create a simple interceptor and how to attach it.
```cpp
    // Create an Interception Settings Struct
    WebXO::InterceptSettings interceptorSettings;
    interceptorSettings.callback = INTERCEPTIONFUNCTION;

    // Init the HTTP class + Set the base path of the HTTP Files
    WebXO::HTTP http(optsCLI.basePath, interceptorSettings);
```

Now the interception function can be attached in various ways as you can see in the previous example ``INTERCEPTIONFUNCTION`` was ``=`` to the ``callback``.
Another example can be:
```cpp
    // Example of a lambda function being used
    interceptorSettings.callback = [=](WebXO::HTTPReq &hReq) -> CompBuffer
    {
        // Example dummy direct buffer
        return CompBuffer((char*)"NULL", -1);
    };
```

As long as the function signature matches then the implementation does not matter.

## InterceptThis | ``./Examples/InterceptThis``
> This example requires NodeJS to be installed and for the ``server.js`` file to be ran.

The example code allows for the NodeJS server to intercept and manipulate all HTTP requests and to attach a new payload to be sent back to the client.




# Notes

## CompBuffer Debugging
To debug a CompBuffer you can use the **new** ``Utils::Memory::Print_Memory()`` function, the method can print out the buffer to stdout for further debugging without running a debugger (``gdb`` or similar).

To use the new util you can simply call the method by:
```cpp
Utils::Memory::Print_Memory(CompBuffer, Rows_to_print);
```

The first parameter is the CompBuffer to print out and the second is the rows to print, if not set the whole buffer will be printed.