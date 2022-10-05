# num_parties=$(($(($1))))

for i in {1..7}
do 
    openssl genrsa -out private0$i.pem 2048
    openssl rsa -in private0$i.pem -outform PEM -pubout -out public0$i.pem
done
