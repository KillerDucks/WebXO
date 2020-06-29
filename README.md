# WebXO
A super simple ~~`but poorly written`~~ web server written in C++.

## Build Status
![AWS CodeBuild](https://codebuild.us-east-1.amazonaws.com/badges?uuid=eyJlbmNyeXB0ZWREYXRhIjoiUmt5Wmd0ME94YkVYWkovVyt1UzB2bjVDQ2RXUmVJK2xxYW5vbjJGZmQ0WkpCbHoxTlhFSnRIbjlGb1BhcVZBWGNLRHQvdG04cjBQcFkvT1NHckZ5c0wwPSIsIml2UGFyYW1ldGVyU3BlYyI6ImZGcTZHUjNUMG9Kakt3VVEiLCJtYXRlcmlhbFNldFNlcmlhbCI6MX0%3D&branch=master)

## Info

The server can be installed locally (not recommended) or in a docker container (highly recommended).

# Installation

## Installation - Local
This method is not recommended as the server uses a Dynamic Library which means installing it to your ``/usr/local/lib`` and setting your ``LD_LIBRARY_PATH`` to reach the `so` file. Which could lead to a broken system if not done correctly.

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

# (Optional) Run the server via make
$> make exec
```

## Installation - Docker
This method is highly recommended and is therefore the prefered way of using the server.

To get the server up and running run the following commands.

```sh
# Pull the docker image from Docker Hub
$> docker pull killerducks/webxo-env:latest

# This will run the test page
$> docker run -d -p 8080:8080 killerducks/webxo-env:latest

# This way will allow you to attach your http folder to the server
$> docker run -d -v <localfilepath>:/etc/webxo/http -p 8080:8080 killerducks/webxo-env:latest
```
