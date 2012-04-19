#include "mysocket.h"
#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct args {
  char *address;
  char *buffer;
  int fd;
};

pthread_mutex_t lock;

void *get_from_server(void *arg) {
  struct args* myarg = (struct args*)arg;

  while(1) {
    bzero(myarg->buffer, 150000);

    int n = 1;
    while(n != 0) {
      n = write(myarg->fd, myarg->buffer, strlen(myarg->buffer));
      printf("%s", myarg->buffer);
      bzero(myarg->buffer, 150000);
    }
  }

  close(myarg->fd);
  free(myarg->buffer);
  free(myarg);
}

void *send_to_server(void *arg) {
  struct args* myarg = (struct args*)arg;
  
  while(1) {
    bzero(myarg->buffer, 150000);
    read(myarg->fd, myarg->buffer, 150000);
  }

  close(myarg->fd);
  free(myarg->buffer);
  free(myarg);
}

int main(int argc, char** argv) {
  int connfd;

  pthread_t read_thread, write_thread;
  struct args* myarg[2];
  pthread_mutex_init(&lock, NULL);

  if(argc < 2) {
    printf("Usage: %s port\n", argv[0]);
    exit(0);
  }
  
  connfd = connectTo("localhost", atoi(argv[1]));
  
  myarg[0] = (struct args*)malloc(sizeof(struct args));
  myarg[0]->buffer = (char *)malloc(sizeof(char)*150000);
  myarg[0]->fd = connfd;
  pthread_create(&read_thread, NULL, send_to_server, myarg[0]);

  myarg[1] = (struct args*)malloc(sizeof(struct args));
  myarg[1]->buffer = (char *)malloc(sizeof(char)*150000);
  myarg[1]->fd = connfd;
  pthread_create(&write_thread, NULL, get_from_server, myarg[1]);

  pthread_join(read_thread, NULL);
  pthread_join(write_thread, NULL);

  pthread_exit(0);
}
