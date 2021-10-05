export DESTDIR="$(pwd)/avr-libc_ubuntu_$1" && make install
dpkg-deb --build --root-owner-group avr-libc_ubuntu_$1
