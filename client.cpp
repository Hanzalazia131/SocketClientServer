#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 



void ConnectionHanling (int sockfd){

    int n;
    char buffer[256];

    while( true ){       //strcmp( buffer, "quit") != 0
            printf("Please enter the message or Quit: ");
            bzero(buffer,256);
            fgets(buffer,255,stdin);
            
            
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0){ 
                perror("ERROR writing to socket");
                break;
            }

            bzero(buffer,256);
            n = read(sockfd,buffer,255);

            if (n < 0) {
                perror("ERROR reading from socket");
                break;
            }
            printf("%s\n",buffer);
        }
    close (sockfd);
    exit(0);
}



int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(0);
    }
    
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
        exit(0);
    }

    ConnectionHanling(sockfd);
    close(sockfd);
    return 0;
}
