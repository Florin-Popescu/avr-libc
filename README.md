# AVR GCC
Standard C library for AVR-GCC

## Changes from official repo
- Added support for ATmega324PB
- Added support for ATmega4808

## Supporting new stuff
This is a rough guide since it only documents changes already performed. So far new devices are still fairly compatibile with existing ones, but this will most likely not always be the case. It is not intendend to be an extensive porting guide.

### New devices
1. Add the device to ./configure.ac
	- to the CHECK_AVR_DEVICE list of macros under its family
	- to the list of makefiles under its family subfolder
2. Add the device to the list of assembly files in ./crt1/iosym/Makefile.am.
3. Add the device to the scripts:
	- ./devtools/gen-avr-lib-tree.sh
	- ./devtools/generate_iosym.sh
4. Add the device to the makefile in ./include/avr/Makefile.am.
5. Add an include for the device header to ./include/avr/io.h.
6. Add the header for the device from the ATmega DFP to ./include/avr.
7. Add the device to the documentation in:
	- ./doc/api/main_page.dox
	- ./doc/api/using-tools.dox

Forked from latest sources at [AVR Libc Project](https://www.nongnu.org/avr-libc/).