 /*Server==================================================*/
   if(argc==1 || argc == 3){
     if(argc==3){
       if(!strcmp("-p",argv[1])){
         sscanf(argv[2],"%i",&port);
       }else{
         printf("Invalid parameter.\nUsage: chat [-p PORT] HOSTNAME\n");
         exit(0);
       }
     }else port=MYPORT;
     
     printf("\n*** Server program starting (enter \"quit\" to stop): \n");
     fflush(stdout);

     /* Create and name a socket for the server */
     server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
     server_address.sin_family = AF_INET;
     server_address.sin_addr.s_addr = htonl(INADDR_ANY);
     server_address.sin_port = htons(port);
     bind(server_sockfd, (struct sockaddr *)&server_address, addresslen);

     /* Create a connection queue and initialize a file descriptor set */
     listen(server_sockfd, 1);
     FD_ZERO(&readfds);
     FD_SET(server_sockfd, &readfds);
     FD_SET(0, &readfds);  /* Add keyboard to file descriptor set */
     
     //struct clientinfo c;
     
     //printf("sizeof(readfds)/(testfds): %d/%d\n",sizeof(readfds),sizeof(testfds));
     //printf("Number of bits in readfds/testfds: %d/%d\n", sizeof(readfds)*8,sizeof(testfds)*8);

     /*  Now wait for clients and requests */
     while (1) {
        testfds = readfds;
        select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
                    
        /* If there is activity, find which descriptor it's on using FD_ISSET */
        for (fd = 0; fd < FD_SETSIZE; fd++) {
           if (FD_ISSET(fd, &testfds)) {
              
              if (fd == server_sockfd) { /* Accept a new connection request */
                 client_sockfd = accept(server_sockfd, NULL, NULL);
                 /*printf("client_sockfd: %d\n",client_sockfd);*/
                
                                
                 if (num_clients < MAX_CLIENTS) {
                    FD_SET(client_sockfd, &readfds);
                    fd_array[num_clients]=client_sockfd;
                    /*Client ID*/
                    printf("Client %d joined\n",num_clients++);
                    fflush(stdout);
                    
                    sprintf(msg,"M%2d",client_sockfd);
                    /*write 2 byte clientID */
                    send(client_sockfd,msg,strlen(msg),0);
                 }
                 else {
                    sprintf(msg, "XSorry, too many clients.  Try again later.\n");
                    write(client_sockfd, msg, strlen(msg));
                    close(client_sockfd);
                 }
              }
              else if (fd == 0)  {  /* Process keyboard activity */                 
                 fgets(kb_msg, MSG_SIZE + 1, stdin);
                 //printf("%s\n",kb_msg);
                 if (strcmp(kb_msg, "quit\n")==0) {
                    sprintf(msg, "XServer is shutting down.\n");
                    for (i = 0; i < num_clients ; i++) {
                       write(fd_array[i], msg, strlen(msg));
                       close(fd_array[i]);
                    }
                    close(server_sockfd);
                    exit(0);
                 }
                 else {
                    //printf("server - send\n");
                    sprintf(msg, "M%s", kb_msg);
                    for (i = 0; i < num_clients ; i++)
                       write(fd_array[i], msg, strlen(msg));
                 }
              }
              else if(fd) {  /*Process Client specific activity*/
                 //printf("server - read\n");
                 //read data from open socket
                 result = read(fd, msg, MSG_SIZE);
                 
                 if(result==-1) perror("read()");
                 else if(result>0){
                    /*read 2 bytes client id*/
                    sprintf(kb_msg,"M%2d",fd);
                    msg[result]='\0';
                    
                    /*concatinate the client id with the client's message*/
                    strcat(kb_msg,msg+1);                                        
                    
                    /*print to other clients*/
                    for(i=0;i<num_clients;i++){
                       if (fd_array[i] != fd)  /*dont write msg to same client*/
                          write(fd_array[i],kb_msg,strlen(kb_msg));
                    }
                    /*print to server*/
                    printf("%s",kb_msg+1);
                    
                     /*Exit Client*/
                    if(msg[0] == 'X'){
                       exitClient(fd,&readfds, fd_array,&num_clients);
                    }   
                 }                                   
              }                  
              else {  /* A client is leaving */
                 exitClient(fd,&readfds, fd_array,&num_clients);
              }//if
           }//if
        }//for
     }//while
  }//end Server code

}//main
