INSTALL_DIR=/c/avr
XMLDIR=$HOME/Documents/Atmel.ATmega_DFP.1.4.351/atdf
export XMLDIR

./bootstrap

cd devtools
./generate_iosym.sh
cd ..

mkdir build
cd build

PATH=$INSTALL_DIR/bin:$PATH
export PATH

../configure \
--prefix=$INSTALL_DIR \
--host=avr \
--datadir=$INSTALL_DIR

make all -j $NUMBER_OF_PROCESSORS
make install
