# Puneet Vohra, Belig Borjiged CIT59 RSA

all:	rsa	client

rsa:	
	gcc -Wall -g rsa.c -o rsa 

client:	
	gcc -Wall -g cli.c -o client 

clean:
	rm *.o 
