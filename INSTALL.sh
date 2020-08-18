#!/bin/bash 

# Just install the build tools
sudo apt-get update && sudo apt-get install build-essential -y

# Make the Dynamic Library and install into /usr/local/lib
sudo make LibWebX

# Make WebXO
make

# (Optional) Set the LD path if not already set
export LD_LIBRARY_PATH=/usr/local/lib

# (Optional) Run the server via make
make exec