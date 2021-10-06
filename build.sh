#!/bin/bash

case $(uname -o | cut -d '/' -f2) in
	"Msys")
		export XMLDIR=$HOME/Downloads/Atmel.ATmega_DFP.1.7.374/atdf

		SUDO=
		;;
	"Linux")
		CONFIG_HOST=

		NUMBER_OF_PROCESSORS=$(nproc)
		SUDO=sudo
		;;
esac

./bootstrap

cd devtools
./generate_iosym.sh
cd ..

mkdir build
cd build

../configure \
--host=avr

make all -j$NUMBER_OF_PROCESSORS
$SUDO make install
