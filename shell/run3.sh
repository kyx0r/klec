


for (( i=0; i<40000; i+=2000))
do

for (( c=0; c<80; c++ ))
do  
	var=$((50899877+(40000*2)+($c*25)+$i))
	python ./base3.py -n 25 -s $var &
done
wait < <(jobs -p)
done
