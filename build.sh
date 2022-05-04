#!/bin/bash

case $(uname -o | cut -d '/' -f2) in
	"Msys")
		SUDO=
		;;
	"Linux")
		CONFIG_HOST=

		NUMBER_OF_PROCESSORS=$(nproc)
		SUDO=sudo
		;;
esac

export XMLDIR=~/Downloads/atdf

./bootstrap

cd devtools
./generate_iosym.sh
cd ..

mkdir build
cd build

../configure \
--host=avr

make all -j$NUMBER_OF_PROCESSORS
