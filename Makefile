# Puneet Vohra, Belig Borjiged CIT59 RSA

all:	rsa	client	server

rsa:	
	gcc -Wall -g rsa.c -o rsa 

client:	mysocket.h
	gcc -Wall -g -lpthread client.c -o client 

server:	mysocket.h
	gcc -Wall -g -lpthread server2.c -o server 

clean:
	rm *.o 
