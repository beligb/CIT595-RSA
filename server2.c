#include <socket.h>
#include "RSALibrary.c"
#include <pthread.h>

int did_it_return = 0;

struct Parameters {
int connfd, firstArg, secondArg, thirdArg, fourthArg, fifthArg;
};

/* Function to send the Keys */

void send_key(struct Parameters *args){
    write(args->connfd, &args->firstArg, sizeof(int));
    write(args->connfd, &args->secondArg, sizeof(int));
    printf("My public key is %d %d\n", args->firstArg, args->secondArg);
    printf("My private key is %d, %d\n", args->thirdArg, args->secondArg);
}

/* Function to receive the Keys */

void receive_key(struct Parameters *args) {
    recv(args->connfd, &args->fourthArg, sizeof(int), 0);
    recv(args->connfd, &args->fifthArg, sizeof(int), 0);
    printf("Friends' public key : %d %d\n", args->fourthArg, args->fifthArg);
}

/* Function to read the data & Decrypt it */
void *read_data(void *fd){
    struct Parameters *args = (struct Parameters *)fd;
    int size_of_buffer = 0;
    int buffer_of_encoding[512];
    char buffer[512];  
    bzero(buffer, 512);
     
    bzero(buffer_of_encoding, 512);
    int i = 0;
    
    while(1) {
    
        size_of_buffer = recv(args->connfd, buffer_of_encoding, sizeof(buffer_of_encoding), 0);
        size_of_buffer /= 4;
        printf("Server recieved the data\n"); 
        printf("size_of_buffer: %d\n", size_of_buffer);  
        
        for (i = 0; i < size_of_buffer; i++) {
                 buffer_of_encoding[i] = ntohl(buffer_of_encoding[i]);
                 printf("%d\n", buffer_of_encoding[i]);
        }
        printf("BEGIN DECRYPTING!!!!!!d=%d, c=%d", args->thirdArg, args->secondArg);
        for(i = 0; i < size_of_buffer; i++) {
            buffer[i] = decrypt(buffer_of_encoding[i], args->thirdArg, args->secondArg );
            printf("buffer[i] = %c\n", buffer[i]);
        
        } 

        if(strcmp(buffer, "quit") == 0) {
	    write(args->connfd, buffer, sizeof(buffer));
            return;    
        }
         bzero(buffer, 512);
      
    
    }
}

/* Function to write the data*/
void *write_data(void *fd) {
    struct Parameters *args = (struct Parameters *)fd;
    char buffer[512];
    bzero(buffer, 512);
    int i = 0;
    while(1) {
        while(1) {
        if((buffer[i] = getchar()) == '\n') {
            buffer[i] = '\0';
            i = 0;
            break;
        }
        else {
            i++;
        }
        }
       
        write(args->connfd, buffer,strlen(buffer));
        if(strcmp(buffer, "quit") == 0) {
          
           return;
       }
        bzero(buffer, 512);
      
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
	   printf("Usage: %s port\n", argv[0]);
	   exit(0);
	}
        first_prime = atoi(argv[2]);
        sec_prime = atoi(argv[3]); 
        generatePrimeNumbers(&first_prime, &sec_prime);
        printf("firstPrime = %d, SecondPrime= %d\n", first_prime , sec_prime);
        int c = first_prime * sec_prime; 
        int t = totient(first_prime*sec_prime);
        int e = coprime(t);  
        int d = mod_inverse(e, (first_prime - 1) * (sec_prime - 1)); 
        printf("c = %d t = %d e = %d d = %d\n", c,t,e,d);


	srvfd = makeListener(atoi(argv[1]));
	connfd = listenFor(srvfd);
        args.connfd = connfd;
        args.firstArg  = e;
        args.secondArg = c;
        args.thirdArg = d;
        send_key(&args);
        receive_key(&args);
        printf("e%d c%d d%d\n", args.firstArg, args.secondArg, args.thirdArg);
        pthread_create(&readThread, NULL, readData, &args);
        pthread_create(&writeThread, NULL, write_data, &args);
        pthread_join(readThread, NULL);
        pthread_cancel(writeThread);
	close(connfd);	
	close(srvfd);
}
