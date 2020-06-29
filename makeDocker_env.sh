#!/bin/bash
echo "Building the Docker Image for WebXServer Development Enviorment"
# docker build --rm . -t webxserver-env:latest
while getopts fte opt; do
    case $opt in
        f) docker build --rm . -t webxserver-env:latest
        ;;
        t) docker build --rm --file Dockerfile.tiny -t webxserver-tiny:latest .
        ;;
        e) docker build --rm --file Dockerfile.exec -t webxserver-exec:latest .
        ;;
    esac
done