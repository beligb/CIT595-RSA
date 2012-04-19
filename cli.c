#include "mysocket.h"

int main(int argc, char** argv) {
	int n;
	int fd;
	char buffer[256];
	
	if(argc < 2) {
	   printf("Usage: %s port\n", argv[0]);
	   exit(0);
	}
	
	fd = connectTo("localhost", atoi(argv[1]));
	
	bzero(buffer, 256);        
        while(fgets(buffer, 256, stdin) != NULL) {
            write(fd, buffer, strlen(buffer));
            bzero(buffer, 256);
        }

        while(read(fd, buffer, sizeof(buffer)) != 0) {
            printf("%s", buffer);
            bzero(buffer, 256);
        }
	
	close(fd);
}
