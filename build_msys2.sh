INSTALL_DIR=/c/avr

./bootstrap

mkdir build
cd build

PATH=$INSTALL_DIR/bin:$PATH
export PATH

./configure \
--prefix=$INSTALL_DIR \
--host=avr \
--datadir=$installdir

make all -j $NUMBER_OF_PROCESSORS
make install
