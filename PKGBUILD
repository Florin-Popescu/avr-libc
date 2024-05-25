pkgname=avr-libc
pkgver=2.0.0
pkgrel=4
epoch=
pkgdesc="Standard C library for AVR-GCC"
arch=('x86_64')
url=""
license=('GPL')
groups=()
depends=()
makedepends=()
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=(https://github.com/Florin-Popescu/avr-libc/archive/refs/tags/v${pkgver}-${pkgrel}.tar.gz)
noextract=()
md5sums=('SKIP')
validpgpkeys=()

prepare() {
	cd "${srcdir}/${pkgname}-${pkgver}-${pkgrel}"
	mkdir build
}

build() {
	cd "${srcdir}/${pkgname}-${pkgver}-${pkgrel}"
	export XMLDIR=~/Downloads/atdf

	./bootstrap

	cd devtools
	./generate_iosym.sh
	cd ..

	cd build
	../configure --host=avr --prefix=/usr
	make all
}

package() {
	cd "${srcdir}/${pkgname}-${pkgver}-${pkgrel}"
	cd build
	make DESTDIR="$pkgdir/" install
}
