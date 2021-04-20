
var=$(cat /lib/modules/5.4.69/modules.order)
for p in $var
do
	modprobe $(basename "$p")
done
