#include <thread>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int SetupSocket(unsigned port_num){
    int sockfd, opt=1;
    struct sockaddr_in serv_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // setsocketopt()   // Make it reuseable
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt));
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_num);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    if( listen(sockfd, 5) ) {
         perror("listen");
         exit(EXIT_FAILURE);
    }
    return sockfd;
}

void HandleNewConnection( int socketfd ) {
    
    cout<< "i am in connection handling"<< endl;
    
    while ( true ) {
        
        char buffer[256];

        cout << "i am in while before read"<< endl; //debuging
        bzero(buffer,256);

        int bytes = read( socketfd, buffer, 256);

        sleep(3);
        cout << "i am in while after read"<< endl; //debugging
        
        int msg = write(socketfd,"massage recieved",strlen("massage recieved"));
        
        cout << "i am in while after msg"<<endl; //debugging
        
        cout << "received " << bytes << " bytes on socket: " << socketfd << endl;
        cout << "received data: " << buffer << endl;
        
        //cout << "i am in while after msg\n";

        if ( bytes < 0 ) {

            cout << "i am in if"<< endl;  //debugging
            cout << "Done. socket: " << socketfd << endl;
            break;
        }
    }
    close(socketfd);
}

int main(int argc, char *argv[]) {
    int sockfd, port_num;
    socklen_t client_socketfd;
     
    char buffer[256];
    struct sockaddr_in client_addr;
     
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided");
         exit(1);
    }
    port_num = atoi(argv[1]);        
    
    sockfd = SetupSocket(port_num);
    
    while(true) {
	socklen_t len;

        int client_socketfd = accept(sockfd, (struct sockaddr *)&client_addr, &len );

        cout<< "i am in main after accept"<<endl;  //debugging
        
        if (client_socketfd < 0) {
            perror("ERROR on accept");
	        exit(1);
	    }

        auto thd = thread( HandleNewConnection, client_socketfd );
        thd.detach();
        break;
    }
    close(sockfd);
    close(client_socketfd);
}

