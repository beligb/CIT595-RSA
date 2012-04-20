#include "mysocket.h"
#include "rsa.h"
#include <pthread.h>

int did_it_return = 0;

struct Parameters {
  int connfd, clientE, clientN, serverD, serverE, serverN;
};

char *numToString(int length) {
  char size[20];
  int character = length;
  int counter = 0;
  while(length > 0) {
    character = length % 10;
    size[counter] = character + 48;
    length = length / 10;
    counter++;
  }
  char *number = (char *)malloc(sizeof(char)*20);
  int index = counter;
  int i;
  for(i = 0; i < index; i++) {
    number[i] = size[counter - 1];
    counter--;
  }
  number[i] = '\0';
  return number;
}

/* Function to send the Keys */
void send_key(struct Parameters *args){
    write(args->connfd, &args->serverN, sizeof(int));
    write(args->connfd, &args->serverE, sizeof(int));
    printf("My public key is %d %d\n", args->serverN, args->serverE);
    printf("My private key is %d\n", args->serverD);
}

/* Function to receive the Keys */
void receive_key(struct Parameters *args) {
    recv(args->connfd, &args->clientN, sizeof(int), 0);
    recv(args->connfd, &args->clientE, sizeof(int), 0);
    printf("The client's public key is %d %d\n", args->clientN, args->clientE);
}

void *read_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    int i, j;
    int size_of_buffer = 0;
    char ascii[20];
    char buffer_of_encoding[512];
    char buffer[512];
    bzero(ascii, 20);
    bzero(buffer, 512);
    bzero(buffer_of_encoding, 512);

    while(1) {
        size_of_buffer = recv(args->connfd, buffer_of_encoding, sizeof(buffer_of_encoding), 0);
        size_of_buffer /= 4;
        printf("Message received from Client: %s\n", buffer_of_encoding);
		
        printf("Client's message decrypted: ");
	j = 0;
        for(i = 0; i < size_of_buffer; i++) {
	  if(buffer_of_encoding[i] != ' ') {
	    ascii[i] = buffer_of_encoding[i];
	  } else {
            buffer[j] = encrypt(atoi(ascii), args->serverD, args->serverN);
	    j++;
	    bzero(ascii, 20);
	  }
        }

        if(strcmp(buffer, "quit") == 0) {
          write(args->connfd, buffer, sizeof(buffer));
          exit(0);
        } else {
	  printf("%s\n\n", buffer);
	}
	
        bzero(buffer, 512);
	bzero(buffer_of_encoding, 512);
    }
}

/* Function to write the data*/
void *write_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    char buffer_of_encoding[512];
    char buffer[512];
    char number[10];
    bzero(buffer, 512);
    bzero(buffer_of_encoding, 512);
    char letter;

    int i = 0;
    int j = 0;
    while(1) {
        while(1) {
	  letter = getchar();
	  if(letter == '\n') {
	    buffer[j] = '\0';
	    i = 0;
	    j = 0;
	    break;
	  } else {
	    bzero(number, 10);
	    strcpy(number, numToString(endecrypt((int)letter, args->clientE, args->clientN)));
	    strcat(buffer, number);
	    j += strlen(number);
	    buffer[j] = ' ';
	    j++;
	    i++;
	  }
	}

	if(strcmp(buffer, "quit") == 0) {
	  exit(0);
	} else {
	  write(args->connfd, buffer, strlen(buffer));
	}

	bzero(buffer, 512);
	bzero(buffer_of_encoding, 512);
    }
}

/* Main function that also spawns the thread */
int main(int argc, char** argv) {
        srand(time(NULL)); 
        pthread_t readThread, writeThread;
        struct Parameters args;
	int srvfd, connfd;
	char buffer[256];
	int first_prime, sec_prime; //the two prime numbers

        if(argc < 4) {
           printf("File PortNum Prime1 Prime2");
	   exit(0);
	}
        first_prime = atoi(argv[2]) + 6;
        sec_prime = atoi(argv[3]) + 6;
        generatePrimeNumbers(&first_prime, &sec_prime);
        generateKeys(first_prime, sec_prime, &args.clientE, &args.serverD, &args.clientN);
        args.run = 1;
        srvfd = makeListener(atoi(argv[1]));
        connfd = listenFor(srvfd);
        args.connfd = connfd;

        send_key(&args);
        receive_key(&args);

        pthread_create(&readThread, NULL, read_data, &args);
        pthread_create(&writeThread, NULL, write_data, &args);

        pthread_join(readThread, NULL);
        pthread_cancel(writeThread);
	close(connfd);	
	close(srvfd);
}
