# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.145.1/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version: debian-10, debian-9
ARG VARIANT="buster"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends   build-essential \
                                                    libgtest-dev \
                                                    cmake \
                                                    zlib1g \
                                                    zlib1g-dev \
    && echo Installing GTest && cd /usr/src/gtest && cmake CMakeLists.txt && make && cp *.a /usr/lib