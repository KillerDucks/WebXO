#!/bin/bash
echo "Building the Docker Image for WebXServer Development Enviorment"
docker build . -t webxserver-env:latest