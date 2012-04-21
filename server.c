#include "mysocket.h"
#include "rsa.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct Parameters {
    int connfd;
    long clientE, clientN, serverD, serverE, serverN;
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
    write(args->connfd, &args->serverN, sizeof(long));
    write(args->connfd, &args->serverE, sizeof(long));
}

/* Function to receive the Keys */
void receive_key(struct Parameters *args) {
    recv(args->connfd, &args->clientN, sizeof(long), 0);
    recv(args->connfd, &args->clientE, sizeof(long), 0);
    printf("The client's public key is %ld %ld\n", args->clientN, args->clientE);
}

/* Function to read the data & Decrypt it */
void *read_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    int i;
    int size_of_buffer = 0;
    char buffer_of_encoding[1024];
    char buffer[1024];
    bzero(buffer, 1024);
    bzero(buffer_of_encoding, 1024);

    while(1) {
        int n = recv(args->connfd, buffer_of_encoding, sizeof(buffer_of_encoding), 0);
        size_of_buffer = n / 4;
        if(size_of_buffer > 0) {
            printf("\nMessage received from Client: %s\n", buffer_of_encoding);
            i = 0;
            char *ptr = strtok(buffer_of_encoding, " ");
            while(ptr != NULL) {
                buffer[i] = (char)decrypt(atoi(ptr), args->serverD, args->serverN);
                ptr = strtok(NULL, " ");
                i++;
            }
            buffer[i] = '\0';

            if(strcmp(buffer, "quit") == 0) {
                printf("Client is quitting.\n");
                break;
            } else {
                printf("Client's message decrypted: ");
                printf("%s\n", buffer);
            }

            bzero(buffer, 1024);
            bzero(buffer_of_encoding, 1024);
        }
    }
}

/* Function to write the data*/
void *write_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    char buffer[1024];
    char number[10];
    bzero(buffer, 1024);
    char letter[1024];

    while(1) {
        int i = 0;
        int j = 0;
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
                strcpy(number, numToString(encrypt((int)letter[i], args->clientE, args->clientN)));
                strcat(buffer, number);
                j += strlen(number);
                buffer[j] = ' ';
                j++;
            }
            i++;
        }

        if(strlen(buffer) > 0) {
            write(args->connfd, buffer, strlen(buffer));
        }

        if(strcmp(letter, "quit") == 0) {
            exit(0);
        }

        bzero(letter, 1024);
        bzero(buffer, 1024);
    }
}

/* Main function that also spawns the thread */
int main(int argc, char** argv) {
    srand(time(NULL));
    pthread_t readThread, writeThread;
    struct Parameters args;
    int srvfd, connfd;
    char value[100];
    bzero(value, 100);

    if(argc < 2) {
        printf("Run server with 'server [port number]'");
        exit(0);
    }

    srvfd = makeListener(atoi(argv[1]));

    while(1) {
        connfd = listenFor(srvfd);

        long first_prime, sec_prime, e, c, d;
        printf("\nEnter two numbers (m n) to generate keys based on the mth and nth primes: ");
        scanf("%ld %ld", &first_prime, &sec_prime);
        printf("\nGenerating keys...\n");
        generatePrimeNumbers(&first_prime, &sec_prime);
        generateKeys(first_prime, sec_prime, &e, &d, &c);
        printf("Your public key (will be sent to server): %ld %ld\n", e, c);
        printf("Your private key (will not be sent): %ld %ld\n", d, c);

        args.connfd = connfd;
        args.serverE = e;
        args.serverN = c;
        args.serverD = d;
        send_key(&args);
        receive_key(&args);

        pthread_create(&readThread, NULL, read_data, &args);
        pthread_create(&writeThread, NULL, write_data, &args);
    }

    close(connfd);
    close(srvfd);
    exit(0);
}
