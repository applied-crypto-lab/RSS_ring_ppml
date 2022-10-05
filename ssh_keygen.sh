#!/bin/bash

# for i in {1..7}
# do 
# openssl genrsa -out private0$i.pem 2048
# openssl rsa -in private0$i.pem -outform PEM -pubout -out public0$i.pem
# done

openssl genrsa -out private01.pem 2048
openssl rsa -in private01.pem -outform PEM -pubout -out public01.pem

openssl genrsa -out private02.pem 2048
openssl rsa -in private02.pem -outform PEM -pubout -out public02.pem

openssl genrsa -out private03.pem 2048
openssl rsa -in private03.pem -outform PEM -pubout -out public03.pem

openssl genrsa -out private04.pem 2048
openssl rsa -in private04.pem -outform PEM -pubout -out public04.pem

openssl genrsa -out private05.pem 2048
openssl rsa -in private05.pem -outform PEM -pubout -out public05.pem

openssl genrsa -out private06.pem 2048
openssl rsa -in private06.pem -outform PEM -pubout -out public06.pem

openssl genrsa -out private07.pem 2048
openssl rsa -in private07.pem -outform PEM -pubout -out public07.pem