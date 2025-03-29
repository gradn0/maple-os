#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom maple.iso &
gdb kernel/maple.kernel -ex "target remote localhost:1234"
