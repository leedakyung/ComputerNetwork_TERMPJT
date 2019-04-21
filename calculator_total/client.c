/*
filename server_ipaddress portno

argv[0] filename
argv[1] server_ipaddress
argv[2] portno
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    if(argc<3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("ERROR opening socket.");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "ERROR, no such host");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("connection failed");
    }

    int num1, num2, choice, ans;

    //error : bzero(buffer,256) >> bzero(buffer,255) modify (OK)
    S:
    bzero(buffer,255);
    n = read(sockfd, buffer, 255); //read server string
    if(n < 0) 
        error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d", &num1); //enter no1
    write(sockfd,&num1,sizeof(int)); //send no1 to server

    bzero(buffer,255);
    n = read(sockfd, buffer, 255); //read server string
    if(n < 0) 
        error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d", &num2); //enter no2
    write(sockfd, &num2, sizeof(int)); //send no2 to server

    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if(n < 0)
        error("ERROR reading from socket");
    printf("Server - %s", buffer);
    scanf("%d", &choice); //Enter choice
    write(sockfd, &choice, sizeof(int)); //send choice to server

        if(choice == 5)
            goto Q;

        read(sockfd, &ans, sizeof(int));
        printf("Server - The answer is: %d\n", ans);

        if(choice != 5)
            goto S;
        

    Q : 
    printf("You have selected to exit. Exit Successful.");
    close(sockfd);
    return 0;
}