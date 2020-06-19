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
    gdb

# Set LDD Library Path
ENV LD_LIBRARY_PATH="/usr/local/lib"