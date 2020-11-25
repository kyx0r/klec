mkdir -p /tmp
modprobe zram num_devices=1
echo "32G" > /sys/block/zram0/disksize
mkfs.ext4 /dev/zram0
mount /dev/zram0 /tmp
