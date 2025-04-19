#!/bin/sh
#-cdrom ./Win10_22H2_English_x64v1.iso \
#-vga [std|cirrus|vmware|qxl|xenfb|tcx|cg3|virtio|none]
#qemu-img create -f qcow2 win.qcow2 500G

qemu-system-x86_64 \
-enable-kvm \
-machine pc-q35-9.0 \
-m 12G \
-smp 4 \
-hda ./win.qcow2 \
-boot d \
-netdev user,id=mynet0,net=192.168.1.0/24,dhcpstart=192.168.1.9 \
-device e1000,netdev=mynet0 \
-vga vmware

exit 0
-device vfio-pci,host=01:00.0 \
-device vfio-pci,host=01:00.1
-global ICH9-LPC.disable_s3=1 \
-global ICH9-LPC.disable_s4=1 \
-device virtio-serial-pci \
-spice port=5930,addr=127.0.0.1,disable-ticketing=on,image-compression=off,seamless-migration=on \
-device virtserialport,chardev=spicechannel0,name=com.redhat.spice.0 \
-chardev spicevmc,id=spicechannel0,name=vdagent \
-audiodev '{"id":"audio1","driver":"spice"}' \
-device '{"driver":"ich9-intel-hda","id":"sound0","bus":"pcie.0","addr":"0x1b"}' \
-device '{"driver":"hda-duplex","id":"sound0-codec0","bus":"sound0.0","cad":0,"audiodev":"audio1"}' \

-audiodev '{"id":"audio1","driver":"spice"}' -spice port=5900,addr=127.0.0.1,disable-ticketing=on,image-compression=off,seamless-migration=on \
-device '{"driver":"ich9-intel-hda","id":"sound0","bus":"pcie.0","addr":"0x1b"}' \
-device '{"driver":"hda-duplex","id":"sound0-codec0","bus":"sound0.0","cad":0,"audiodev":"audio1"}' \
