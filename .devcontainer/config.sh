#!/bin/bash
echo "Configuring WebXO Dev Env for DevContainer"
cd /workspaces/WebXO && mkdir ./build && cd ./build && cmake -DDEMO=true -G "Unix Makefiles" ../ && cd ../ && cmake --build /var/WebXO/build --config Debug --target all -- -j && cd ./build && make install && sudo ldconfig