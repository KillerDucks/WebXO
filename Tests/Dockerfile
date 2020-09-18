FROM alpine:latest

# Label
LABEL come.melissa.version="1.3.0"
LABEL vendor="Melissa"

# Get all of the packages we need to build the app
RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    gtest-dev \
    zlib-dev

# Set LDD Library Path
ENV LD_LIBRARY_PATH="/usr/local/lib"

# Make a Build Directory
RUN mkdir -p /var/WebXO/

# Copy in the build files
COPY . /var/WebXO/

# Make the server + lib
RUN cd /var/WebXO/ && mkdir build && cd build && cmake -G "Unix Makefiles" ../ && cd ../ && cmake --build /var/WebXO/build --config Release --target all -- -j && cd build/ && make install

# Run the tests
RUN cd /var/WebXO/build/ && ctest --progress --output-on-failure -O /var/WebXO/CTest.log && cat /var/WebXO/CTest.log 

# Start the webserver
ENTRYPOINT [ "sh", "./var/WebXO/exec.sh" ]