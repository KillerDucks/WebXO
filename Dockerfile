FROM alpine:latest as build-env

# Label
LABEL come.melissa.version="0.1.0_timy-alpha"
LABEL vendor="Melissa"

# Get all of the packages we need to build the app
RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    zlib-dev

# Make a Build Directory
RUN mkdir -p /var/WebXO/

# Copy in the build files
COPY . /var/WebXO/

# Make the server + lib
RUN cd /var/WebXO/ && mkdir build && cd build && cmake -G "Unix Makefiles" ../ && cd ../ && cmake --build /var/WebXO/build --config Release --target all -- -j 
#  && cd /var/WebXO/build && make install

# RUN apk del \
#     g++ \
#     cmake \
#     make

# Now build the demo container
FROM alpine:latest as final-env

# Label
LABEL come.melissa.version="1.3.4"
LABEL vendor="Melissa"

# Install needed dependancies
RUN apk add --no-cache \
        libstdc++ \
        zlib-dev

# Set LDD Library Path
ENV LD_LIBRARY_PATH="/usr/local/lib"

# Copy only the needed files
COPY --from=build-env /var/WebXO/build/EXE/WebXOSrv  /usr/local/bin/WebXOSrv
COPY --from=build-env /var/WebXO/build/LIBS/libWebXO.so  /var/WebXO/libWebXO.so.1.0
COPY --from=build-env /var/WebXO/exec.sh  /tmp/exec.sh

# Copy in the html example site
COPY ./ExampleSite /var/www/ExampleSite
COPY ./ErrorPages /var/www/ErrorPages

# Make the server + lib
RUN  cp /var/WebXO/libWebXO.so.1.0 /usr/local/lib && ln -sf /usr/local/lib/libWebXO.so.1.0 /usr/local/lib/libWebXO.so.1 && ln -sf /usr/local/lib/libWebXO.so.1.0 /usr/local/lib/libWebXO.so && rm -rf /var/WebXO && du -h /

# # DEBUG
# RUN ldd /usr/local/bin/WebXOSrv
# RUN du -h /home/WebXO-Dev/
# RUN ls -la /home/WebXO-Dev/build/obj/

# Expose the port
EXPOSE 8080

# Start the webserver
ENTRYPOINT [ "sh", "./tmp/exec.sh" ]

