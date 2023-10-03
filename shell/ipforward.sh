#!/bin/bash

# Share Wifi with Eth device
#
#
# This script is created to work with Raspbian Stretch
# but it can be used with most of the distributions
# by making few changes.
#
# Make sure you have already installed `dnsmasq`
# Please modify the variables according to your need
# Don't forget to change the name of network interface
# Check them with `ifconfig`

ip_address="192.168.2.1"
netmask="255.255.255.0"
dhcp_range_start="192.168.2.2"
dhcp_range_end="192.168.2.100"
dhcp_time="12h"
eth="eth0"
wlan="wlan0"

ip addr flush dev eth0
systemctl stop dhcpcd

#systemctl start network-online.target &> /dev/null

iptables -F
iptables -t nat -F
iptables -t nat -A POSTROUTING -o $wlan -j MASQUERADE
iptables -A FORWARD -i $wlan -o $eth -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i $eth -o $wlan -j ACCEPT

sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

ifconfig $eth $ip_address netmask $netmask

# Remove default route created by dhcpcd
ip route del 0/0 dev $eth &> /dev/null

systemctl stop dnsmasq

rm -rf /etc/dnsmasq.conf

#rm -rf /etc/dnsmasq.d/* &> /dev/null

echo -e "interface=$eth\n\
bind-interfaces\n\
server=8.8.8.8\n\
domain-needed\n\
bogus-priv\n\
dhcp-range=$dhcp_range_start,$dhcp_range_end,$dhcp_time" > /etc/dnsmasq.conf

systemctl start dnsmasq
