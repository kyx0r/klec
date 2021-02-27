#!/bin/sh
# Migrates root file system to RAM
# --- Needs busybox version of pivot_root ---
# run it like so . migr.sh the "." is does the shell sourcing
# or make sure to run cd / yourself. It needs to become aware of
# the new root by changing shell wd.
# The /sys/block/zram0/disksize does not really matter what size
# it's just a limiter, so set to anything except below the amount
# of your ram. The kernel does not do deallocations when you remove
# files from the zram block, to reset it run "echo 1 > /sys/block/zram0/reset"
# ignore the busy error, it works either way. 
# You may need to adjust the script directories, but the most
# essential ones are alredy here to have a working system.
#
# You can also do this onto tmpfs instead of zram,
# just uncomment the line with tmpfs and comment the 
# mkfs.ext4 and the zram stuff

usrdirs="bin sbin lib32 lib64 lib share include local libexec"
rootdirs="etc root home var"

function copy()
{
	mkdir -p /ramroot
	modprobe zram num_devices=1
	echo "32G" > /sys/block/zram0/disksize
	mkfs.ext4 /dev/zram0
	mount /dev/zram0 /ramroot
	#mount -o size=8G -t tmpfs tmpfs /ramroot
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
        echo "* copy            copy rootfs system to ram"
        echo "* pivot           pivot rootfs"
}

"$@"
help
