num_parties=$(($(($1))))

for i in $(eval echo {1..$num_parties})
do 
    openssl genrsa -out private0$i.pem 2048
    openssl rsa -in private0$i.pem -outform PEM -pubout -out public0$i.pem
done
