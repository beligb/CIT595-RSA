// CLIENT
#include "mysocket.h"
#include "rsab.h"
#include <pthread.h>
#include <math.h>

struct Parameters {
    int connfd, clientE, clientN, clientD, serverE, serverN;
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
    write(args->connfd, &args->clientN, sizeof(int));
    write(args->connfd, &args->clientE, sizeof(int));
    printf("My public key is %d %d\n", args->clientN, args->clientE);
    printf("My private key is %d\n", args->clientD);
}

/* Function to receive the Keys */
void receive_key(struct Parameters *args) {
    recv(args->connfd, &args->serverN, sizeof(int), 0);
    recv(args->connfd, &args->serverE, sizeof(int), 0);
    printf("The server's public key is %d %d\n", args->serverN, args->serverE);
}

/* Function to read the data & Decrypt it */
void *read_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    int i;
    int size_of_buffer = 0;
    char buffer_of_encoding[512];
    char buffer[512];
    bzero(buffer, 512);
    bzero(buffer_of_encoding, 512);

    while(1) {
        recv(args->connfd, buffer_of_encoding, sizeof(buffer_of_encoding), 0);
        size_of_buffer = strlen(buffer_of_encoding);
        printf("\nMessage received from Server: %s\n", buffer_of_encoding);

        printf("Server's message decrypted: ");
	i = 0; 
	char *ptr = strtok(buffer_of_encoding, " ");
	while(ptr != NULL) {
            buffer[i] = (char)endecrypt(atoi(ptr), args->clientD, args->clientN);
            ptr = strtok(NULL, " ");
            i++;
	}
	buffer[i] = '\0';
	printf("%s\n", buffer);
        printf("Please enter a message to send (Type 'quit' to quit): ");

        if(strcmp(buffer, "quit") == 0) {
            write(args->connfd, buffer, strlen(buffer));
            break;
        }

        bzero(buffer, 512);
	bzero(buffer_of_encoding, 512);
    }
    exit(0);
}

/* Function to write the data*/
void *write_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    char buffer_of_encoding[512];
    char buffer[512];
    char number[10];
    bzero(buffer, 512);
    bzero(buffer_of_encoding, 512);
    char letter[512];

    int i = 0;
    int j = 0;

    while(1) {
        printf("Please enter a message to send (Type 'quit' to quit): ");
        while(1) {
            letter[i] = getchar();
            if(letter[i] == '\n') {
                buffer[j] = '\0';
                letter[i] = '\0';
                j = 0;
                i = 0;
                break;
            } else {
                bzero(number, 10);
                strcpy(number, numToString(endecrypt((int)letter[i], args->serverE, args->serverN)));
                strcat(buffer, number);
                j += strlen(number);
                buffer[j] = ' ';
                j++;
            }
            i++;
	}

	write(args->connfd, buffer, strlen(buffer));
        if(strcmp(letter, "quit") == 0) {
            break;
        }

	bzero(letter, 512);
	bzero(buffer, 512);
	bzero(buffer_of_encoding, 512);
    }
    exit(0);
}

/* Main function that also spawns the thread */
int main(int argc, char** argv) {
    srand(time(NULL));
    pthread_t readThread, writeThread;
    struct Parameters args;
    int connfd;
    int first_prime, sec_prime; //the two prime numbers

    if(argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        exit(0);
    }

    connfd = connectTo("localhost", atoi(argv[1]));

    first_prime = 11;
    sec_prime = 13;
    printf("firstPrime = %d, SecondPrime= %d\n", first_prime , sec_prime);
    int c = first_prime * sec_prime;
    int t = totient(first_prime * sec_prime);
    int e = rand() % t;
    while(gcd(e, t) != 1) {
        e = rand() % t;
    }
    int d = mod_inverse(e, t);
    printf("c = %d t = %d e = %d d = %d\n", c,t,e,d);

    args.connfd = connfd;
    args.clientE = e;
    args.clientN = c;
    args.clientD = d;
    receive_key(&args);
    send_key(&args);

    printf("e = %d c = %d d = %d\n", args.clientE, args.clientN, args.clientD);
    pthread_create(&writeThread, NULL, write_data, &args);
    pthread_create(&readThread, NULL, read_data, &args);

    pthread_join(writeThread, NULL);
    pthread_join(readThread, NULL);
    close(connfd);
    exit(0);
}
