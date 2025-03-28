#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/maple.kernel isodir/boot/maple.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Maple" {
	multiboot /boot/maple.kernel
}
EOF
grub-mkrescue -o maple.iso isodir
