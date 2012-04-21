# Puneet Vohra, Belig Borjiged CIT59 RSA

all:	rsa	client	server bruteforce

bruteforce: 
        gcc -Wall -g bruteforce.c -o bruteforce

rsa:	rsa.h
	gcc -Wall -g rsa.c -o rsa 

client:	mysocket.h
	gcc -Wall -g -lpthread client.c -o client 

server:	mysocket.h
	gcc -Wall -g -lpthread server.c -o server 

clean:
	rm *.o 
