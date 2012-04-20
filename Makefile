# Puneet Vohra, Belig Borjiged CIT59 RSA

all:	rsa	client	server

rsa:	
	gcc -Wall -g rsa.c -o rsa 

client:	mysocket.h
	gcc -Wall -g -lpthread -lm client.c -o client 

server:	mysocket.h
	gcc -Wall -g -lpthread -lm server.c -o server 

clean:
	rm *.o 
