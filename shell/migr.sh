#!/bin/bash
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
# If you have /tmp as a mounted point uncomment the 
# mount --rbind /tmp /ramroot/tmp bellow

mkdir -p /ramroot
modprobe zram num_devices=1
echo "32G" > /sys/block/zram0/disksize
mkfs.ext4 /dev/zram0
mount /dev/zram0 /ramroot
#mount -o size=8G -t tmpfs tmpfs /ramroot
cp -a /{bin,etc,sbin,lib64,lib32,lib} /ramroot
mkdir /ramroot/usr
cp -a /usr/{bin,sbin,lib32,lib64,lib,share,include,local,libexec} /ramroot/usr &
cp -a /{root,home,var} /ramroot &
umount /boot
mkdir /ramroot/{dev,proc,sys,run,tmp}
mount --rbind /sys /ramroot/sys
mount --rbind /run /ramroot/run
#mount --rbind /tmp /ramroot/tmp
mount --rbind /dev /ramroot/dev
mount --rbind /proc /ramroot/proc
mkdir /ramroot/oldroot
wait < <(jobs -p)
pivot_root /ramroot /ramroot/oldroot
umount /oldroot -f -l
cd /
