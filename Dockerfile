FROM ubuntu:latest

# Label
LABEL come.melissa.version="0.1.0-alpha"
LABEL vendor="Melissa"

RUN apt-get update
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y install tzdata

# Get all of the packages we need to build the app
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    zlib1g-dev \
    gdb \ 
    curl

# Set LDD Library Path
ENV LD_LIBRARY_PATH="/usr/local/lib"

# Make a Build Directory
RUN mkdir -p /home/WebXO-Dev/

# Copy in the build files
COPY . /home/WebXO-Dev/

# Make the server + lib
RUN cd /home/WebXO-Dev/ && make clean && make LibWebX && make

# Expose the port
EXPOSE 8080

# Start the webserver
CMD [ "./home/WebXO-Dev/exec.sh" ]
