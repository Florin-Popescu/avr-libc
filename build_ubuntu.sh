#!/bin/bash

XMLDIR=$HOME/Downloads/atdf
export XMLDIR

./bootstrap

cd devtools
./generate_iosym.sh

mkdir ../build
cd ../build

../configure \
--host=avr \

make all -j$(nproc)
make install
