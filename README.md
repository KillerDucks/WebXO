# WebXO ![AWS CodeBuild](https://codebuild.us-east-1.amazonaws.com/badges?uuid=eyJlbmNyeXB0ZWREYXRhIjoiUmt5Wmd0ME94YkVYWkovVyt1UzB2bjVDQ2RXUmVJK2xxYW5vbjJGZmQ0WkpCbHoxTlhFSnRIbjlGb1BhcVZBWGNLRHQvdG04cjBQcFkvT1NHckZ5c0wwPSIsIml2UGFyYW1ldGVyU3BlYyI6ImZGcTZHUjNUMG9Kakt3VVEiLCJtYXRlcmlhbFNldFNlcmlhbCI6MX0%3D&branch=master)
A super simple *`but poorly written`* web server written in C++. 

``Complete Package (Exec + Library) @ 616K``
> [v1.2.0] Codename VirtualizeEverything

# Installation
> Note: Currently everything is built with debug support this will be addressed in a future Makefile update, currently you will have to manually change the flags in the Makefile

The server can be installed locally (not recommended) or in a docker container (highly recommended).

## Install Script
This method is not recommended as the server uses a Dynamic Library which means installing it to your ``/usr/local/lib`` and setting your ``LD_LIBRARY_PATH`` to reach the `so` file. This can lead to a broken system if not done correctly (this is not my fault so don't expect an issue regarding this to be resolved).

Below is the manual way to install the Lib and make the files however there is now an install script which is all these commands in one file **without** that does not perform any type of checking and is located in the root of the project at ``/<ProjectPath>/INSTALL.sh``

```sh
# Clone the repo
$> git clone https://github.com/KillerDucks/WebX.git

# cd into the repo
$> cd WebX

# Make the Dynamic Library and install into /usr/local/lib
$> sudo make LibWebX

# Make WebXO
$> make

# (Optional) Set the LD path if not already set
$> export LD_LIBRARY_PATH=/usr/local/lib

# (Optional) Run the server
$> ./build/debug/WebXServer --help
```

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
$> docker pull killerducks/webxo-env:latest

# This will run the test page
$> docker run -d -p 8080:8080 killerducks/webxo-env:latest

# This way will allow you to attach your http folder to the server
$> docker run -d -v <localfilepath>:/etc/webxo/http -p 8080:8080 killerducks/webxo-env:latest
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