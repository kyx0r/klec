#!/bin/sh
# Migrates root file system to RAM
# run it like so . migr.sh the "." is does the shell sourcing
# or make sure to run cd / yourself. It needs to become aware of
# the new root by changing shell wd.
# The /sys/block/zram0/disksize does not really matter what size
# it's just a limiter, so set to anything except below the amount
# of your ram.
# You may need to adjust the script directories, but the most
# essential ones are alredy here to have a working system.
#
# You can also do this onto tmpfs instead of zram

usrdirs="bin sbin lib32 lib64 lib share include local libexec"
rootdirs="etc root home var"

function copy()
{
	cp -a /bin /ramroot
	cp -a /sbin /ramroot
	cp -a /lib64 /ramroot
	cp -a /lib32 /ramroot
	cp -a /lib /ramroot
	mkdir /ramroot/usr
	echo "$usrdirs" | tr ' ' '\n' | while read item; do
		cp -a /usr/$item /ramroot/usr &
	done
	echo "$rootdirs" | tr ' ' '\n' | while read item; do
		cp -a /$item /ramroot/ &
	done
	mkdir /ramroot/dev /ramroot/proc /ramroot/sys \
	/ramroot/run /ramroot/tmp /ramroot/cache /ramroot/oldroot
	echo "please wait for job completion ..."
	exit 0
}

function tcopy()
{
	mkdir -p /ramroot
	mount -o size=32G -t tmpfs tmpfs /ramroot
	copy()
}

function zcopy()
{
	mkdir -p /ramroot
	modprobe zram num_devices=1
	echo "32G" > /sys/block/zram0/disksize
	mkfs.ext4 /dev/zram0
	mount /dev/zram0 /ramroot
	copy()
}

function ztrim()
{
	fstrim /
	exit 0
}

function pivot()
{
	umount /boot
	mount --rbind /sys /ramroot/sys
	mount --rbind /run /ramroot/run
	#mount --rbind /tmp /ramroot/tmp
	mount --rbind /dev /ramroot/dev
	mount --rbind /proc /ramroot/proc
	pivot_root /ramroot /ramroot/oldroot
	umount /oldroot -f -l
	cd /
	exit 0
}

function help()
{
	echo " "
        echo "=====Command Line Options====="
        echo "* zcopy           copy rootfs system to ram (zram0)"
        echo "* ztrim           deallocate pages on unused data on zram from ext4"
        echo "* tcopy           copy rootfs system to ram (tmpfs)"
        echo "* pivot           pivot rootfs"
}

"$@"
help
