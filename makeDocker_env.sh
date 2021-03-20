#!/bin/bash
echo "Building the Docker Image for webxo Development Enviorment"
# docker build --rm . -t webxo-env:latest
while getopts fte opt; do
    case $opt in
        f) docker build --rm . -t webxo-env:dev
        ;;
        t) docker build --rm --file Dockerfile.tiny -t webxo-tiny:dev .
        ;;
        e) docker build --rm --file Dockerfile.exec -t webxo-exec:dev .
        ;;
    esac
done