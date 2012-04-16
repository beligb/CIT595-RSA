# Puneet Vohra, Belig Borjiged CIT59 RSA

all:	rsa

server:	
	gcc -Wall -g rsa.c -o rsa 

clean:
	rm *.o 