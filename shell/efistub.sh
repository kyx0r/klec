#!/bin/sh
mount -t efivarfs none /sys/firmware/efi/efivars/
efibootmgr -b 0000 --delete-bootnum
efibootmgr \
         --create \
         --disk     /dev/nvme0n1 \
         --part     1 \
         --label    KISS \
         --loader   /vmlinuz-6.14.10 \
         --unicode  'root=/dev/nvme0n1p2 amdgpu.ppfeaturemask=0xffffffff' \
         --verbose
