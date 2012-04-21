#include "mysocket.h"
#include "rsa.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct Parameters {
    int connfd;
    long clientE, clientN, clientD, serverE, serverN;
};

char *numToString(long length) {
    char size[100];
    long character = length;
    int counter = 0;
    while(length > 0) {
        character = length % 10;
        size[counter] = character + 48;
        length = length / 10;
        counter++;
    }
    char *number = (char *)malloc(sizeof(char)*100);
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
    write(args->connfd, &args->clientN, sizeof(long));
    write(args->connfd, &args->clientE, sizeof(long));
}

/* Function to receive the Keys */
void receive_key(struct Parameters *args) {
    recv(args->connfd, &args->serverN, sizeof(long), 0);
    recv(args->connfd, &args->serverE, sizeof(long), 0);
    printf("The server's public key is %ld %ld\n", args->serverN, args->serverE);
}

/* Function to read the data & Decrypt it */
void *read_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    int i;
    int size_of_buffer = 0;
    char buffer_of_encoding[1024];
    char saved_encoding[1024];
    char buffer[1024];
    bzero(buffer, 1024);
    bzero(buffer_of_encoding, 1024);

    while(1) {
        int n = recv(args->connfd, buffer_of_encoding, sizeof(buffer_of_encoding), 0);
        strcpy(saved_encoding, buffer_of_encoding);
        size_of_buffer = n / 4;
        if(size_of_buffer > 0) {
            printf("\nMessage received from Server: %s\n", buffer_of_encoding);
            i = 0;
            char *ptr = strtok(buffer_of_encoding, " ");
            while(ptr != NULL) {
                buffer[i] = (char)decrypt(atoi(ptr), args->clientD, args->clientN);
                ptr = strtok(NULL, " ");
                i++;
            }
            buffer[i] = '\0';

            if(strcmp(buffer, "quit") == 0) {
                break;
            } else {
                printf("Server's message decrypted: ");
                printf("%s\n", buffer);
            }

            bzero(buffer, 1024);
            bzero(buffer_of_encoding, 1024);
        }
    }
    close(args->connfd);
    exit(0);
}

/* Function to write the data*/
void *write_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    char buffer[1024];
    char number[10];
    bzero(buffer, 1024);
    char letter[1024];

    int i = 0;
    int j = 0;
    while(1) {
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
                strcpy(number, numToString(encrypt((int)letter[i], args->serverE, args->serverN)));
                if(strlen(buffer) == 0) {
                    strcpy(buffer, number);
                } else {
                    strcat(buffer, number);
                }
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

        bzero(letter, 1024);
        bzero(buffer, 1024);
    }
    close(args->connfd);
    exit(0);
}

/* Main function that also spawns the thread */
int main(int argc, char** argv) {
    srand(time(NULL));
    pthread_t readThread, writeThread;
    struct Parameters args;
    int connfd;
    long first_prime, sec_prime, e, c, d;

    if(argc < 3) {
        printf("Run client with 'client [host name] [port number]'\n");
        exit(0);
    }

    char host[20];
    strcpy(host, argv[1]);
    connfd = connectTo(host, atoi(argv[2]));

    printf("\nEnter two numbers (m n) to generate keys based on the mth and nth primes: ");
    scanf("%ld %ld", &first_prime, &sec_prime);
    printf("\nGenerating keys...\n");
    generatePrimeNumbers(&first_prime, &sec_prime);
    generateKeys(first_prime, sec_prime, &e, &d, &c);
    printf("Your public key (will be sent to server): %ld %ld\n", e, c);
    printf("Your private key (will not be sent): %ld %ld\n", d, c);

    args.connfd = connfd;
    args.clientE = e;
    args.clientN = c;
    args.clientD = d;
    receive_key(&args);
    send_key(&args);

    pthread_create(&writeThread, NULL, write_data, &args);
    pthread_create(&readThread, NULL, read_data, &args);

    pthread_join(writeThread, NULL);
    pthread_join(readThread, NULL);

    close(connfd);
    exit(0);
}
