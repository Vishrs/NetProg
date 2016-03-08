#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define PORT "12345"

int main(int argc, char * argv[]) {
    
    if (argc < 5) {
        printf("ERROR: invalid arguments\n");
        printf("USAGE: : ./a.out <rendezvous server address> <your peer ID> <contact peer ID> <message>\n");
        return EXIT_FAILURE;
    }
    
    char ownPeerID[80] = argv[2];
    ownPeerID[strlen(ownPeerID)] = '\0';
    
    char registerID[80];
    
    strcat("register ", ownPeerID);
    
    struct sockaddr_in server;
    struct addrinfo adrinfo;
    struct addrinfo adrresults;
    
    int sd, length;
    
    if((sd = socket( AF_INET6, SOCK_DGRAM) < 0) {
        perror("socket failed\n");
        return EXIT_FAILURE;
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(12345);
    
    memset(&adrinfo, 0, sizeof(adrinfo));
    adrinfo.ai_family = AF_INET6;
    adrinfo.ai_socktype = SOCK_DGRAM;
    adrinfo.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
    
    int rend_info = getaddrinfo(argv[1], PORT, &adrinfo, &adrresults);
    
    if (rend_info < 0) {
        perror("getaddrinfo() failed\n");
        return EXIT_FAILURE;
    }
    
    
    return EXIT_SUCCESS;
    
}