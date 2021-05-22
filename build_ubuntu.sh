#!/bin/bash

XMLDIR=$HOME/Downloads/atdf
export XMLDIR

chmod +x ./bootstrap
chmod +x ./devtools/gen-avr-lib-tree.sh
./bootstrap

cd devtools
chmod +x ./generate_iosym.sh
./generate_iosym.sh
cd ..

mkdir build
cd build

chmod +x ../configure
../configure \
--host=avr \

make all -j$(nproc)
make install
