#!/bin/bash

sudo cp /etc/dhcpcd.conf /etc/dhcpcd_org.conf
sudo cp /etc/dhcpcd_unlock.conf /etc/dhcpcd.conf

#sudo systemctl restart dhcpcd

echo "Ready"

sleep 50

sudo systemctl restart dhcpcd

sleep 30

sudo systemctl restart dhcpcd

sleep 30

sudo cp /etc/dhcpcd_org.conf /etc/dhcpcd.conf

sudo create_ap --freq-band 5 --daemon -m nat wlan0 eth0 Frustum adapter676
