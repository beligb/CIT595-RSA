# Puneet Vohra, Belig Borjiged CIT59 RSA

all:	rsa	client	server	bruteforce

bruteforce: rsa.h
	gcc -Wall -g bruteforce.c -o bruteforce

client:	mysocket.h	rsa.h
	gcc -Wall -g -lpthread client.c -o client 

server:	mysocket.h	rsa.h
	gcc -Wall -g -lpthread server.c -o server 

clean:
	rm *.o 
