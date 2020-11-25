
howmany() { echo $#; }

var=$(ls /tmp/illusts)

num=$(howmany $var)
a=($(echo "$var" | tr ' ' '\n'))

echo "$num"
echo "" > /tmp/entropy


for (( c=0; c<$num; ))
do

for (( t=0; t<5; t++ ))
do
p1=${a[$c+$t]}
ss="%[entropy] /tmp/illusts/"
ss+=$p1 
ss+=" \n"
convert /tmp/illusts/$p1 -print "$ss" null: >> /tmp/entropy &
done

c=$((c+5))
wait < <(jobs -p)
done

echo "$(sort -g -r -k1 /tmp/entropy)" > /tmp/entropy
feh --title %f___[%wX%h]_[%u/%l] $(cat /tmp/entropy | awk '{ print $2 }')
