#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel

set -e
if [ ! -d build ]; then
    mkdir build
fi
cd build
cmake ..
make
