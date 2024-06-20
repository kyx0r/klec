#!/bin/sh

INTERFACE=wlan0
SSID=RDBNXP
WPA_PASSPHRASE=""
HOSTAPDCONF="/tmp/hostapd.conf"
DNSMASQCONF="/tmp/dnsmasq.conf"

if [ $# -lt 1 ]; then
	echo "./master.sh [bs]top/[bs]tart interface ssid wpa_passphrase"
	exit 0
fi

if [ "$1" = "start" ] || [ "$1" = "bstart" ]; then
	killall hostapd 2>/dev/null
	killall dnsmasq 2>/dev/null

	if [ "$#" -gt 1 ]; then
		INTERFACE=$2
	fi

	if [ "$#" -gt 2 ]; then
		SSID=$3
	fi

	if [ "$#" -gt 3 ]; then
		WPA_PASSPHRASE=$4
	fi

	echo "interface=$INTERFACE" > $HOSTAPDCONF
	echo "driver=nl80211" >> $HOSTAPDCONF
	echo "ssid=$SSID" >> $HOSTAPDCONF
	echo "hw_mode=g" >> $HOSTAPDCONF
	echo "channel=9" >> $HOSTAPDCONF
	echo "auth_algs=1" >> $HOSTAPDCONF
	echo "macaddr_acl=0" >> $HOSTAPDCONF
	echo "beacon_int=1000" >> $HOSTAPDCONF
	echo "dtim_period=2" >> $HOSTAPDCONF
	echo "max_num_sta=3" >> $HOSTAPDCONF
	if [ ! -z $WPA_PASSPHRASE ]; then
		echo "wpa=2" >> $HOSTAPDCONF
		echo "wpa_key_mgmt=WPA-PSK" >> $HOSTAPDCONF
		echo "wpa_passphrase=$WPA_PASSPHRASE" >> $HOSTAPDCONF
	fi

cat <<EOF > $DNSMASQCONF
interface=$INTERFACE
server=192.168.200.1
listen-address=10.100.0.13
dhcp-range=192.168.200.2,192.168.200.130,12h
log-queries
log-dhcp
EOF

	ifconfig $INTERFACE down
	iwconfig $INTERFACE mode managed
	ifconfig $INTERFACE 192.168.200.1 netmask 255.255.255.0 broadcast 192.168.200.255 up
	sleep 2
	dnsmasq -d -p 53 -C $DNSMASQCONF &
	sleep 5
	hostapd $HOSTAPDCONF &
	sleep 5
	iptables -t nat -F
	iptables -A INPUT -i $INTERFACE -j ACCEPT
	iptables -A OUTPUT -j ACCEPT
	iptables -A FORWARD -i $INTERFACE -j ACCEPT
	if [ "$1" = "bstart" ]; then
		iptables --table nat --append POSTROUTING --out-interface enp0s8 -j MASQUERADE
		sleep 2
		iptables --append FORWARD --in-interface enp0s8 -j ACCEPT
		sleep 2
		echo 1 | tee /proc/sys/net/ipv4/ip_forward
	fi
fi

if [ "$1" = "stop" ] || [ "$1" = "bstop" ]; then
	killall hostapd 2>/dev/null
	killall dnsmasq 2>/dev/null
	if [ "$1" = "bstop" ]; then
		echo 0 | tee /proc/sys/net/ipv4/ip_forward
		sleep 1
		iptables -F
		iptables -X
	fi
	echo "DONE!"
fi
