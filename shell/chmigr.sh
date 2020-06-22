#
#!/bin/bash
#Migrates root file system to RAM
#

mkdir -p /ramroot
modprobe zram num_devices=1
echo "8G" > /sys/block/zram0/disksize
mkfs.ext4 /dev/zram0
mount /dev/zram0 /ramroot
#mount -o size=8G -t tmpfs tmpfs /ramroot
cp -a /{bin,etc,sbin,lib64,lib32,lib} /ramroot
mkdir /ramroot/usr
cp -a /usr/{bin,sbin,lib32,lib64,lib,share,include,local} /ramroot/usr &
cp -a /{root,home,var} /ramroot &
umount /boot
mkdir /ramroot/{dev,proc,sys,run,tmp}
mount --rbind /sys /ramroot/sys
mount --rbind /run /ramroot/run
mount --rbind /tmp /ramroot/tmp
mount --rbind /dev /ramroot/dev
mount --rbind /proc /ramroot/proc
wait < <(jobs -p)
chroot /ramroot
