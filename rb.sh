
sudo ip addr add 10.1.10.2 dev eth0
sudo ip route add 10.1.10.3 via 10.1.10.2 dev eth0
sudo ssh alarm@10.1.10.3
sudo ip route del 10.1.10.3
sudo ip addr del 10.1.10.2/32 dev eth0
