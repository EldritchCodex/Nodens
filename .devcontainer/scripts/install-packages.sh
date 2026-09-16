#!/usr/bin/env bash
#
# Installs the system packages needed to build and debug Nodens on top of
# a bare Arch Linux base image, then cleans the package cache so the layer
# doesn't carry unnecessary weight.
#
# Intended to run as root during the container image build (see Dockerfile).

set -euo pipefail

# Refresh package databases and upgrade the base image first, so the
# packages below are installed against an up-to-date system.
pacman -Syyu --noconfirm

pacman -S --noconfirm \
    sudo \
    gdb \
    clang \
    cmake \
    ninja \
    git \
    curl \
    glfw \

# Drop downloaded package archives/databases; they're only needed during
# installation and would otherwise bloat the image.
pacman -Scc --noconfirm
