#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define PORT "12345"

void sendMessageUDP(char *id, int sd, char *servaddr) {
    struct addrinfo adrinfo;
    struct addrinfo *adrresults;
    
    memset(&adrinfo, 0, sizeof(adrinfo));
    adrinfo.ai_family = AF_INET6;
    adrinfo.ai_socktype = SOCK_DGRAM;
    adrinfo.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
    
    int rend_info = getaddrinfo(servaddr, PORT, &adrinfo, &adrresults);
    
    if (rend_info < 0) {
        perror("getaddrinfo() failed\n");
        return;
    }
    
    int snd = sendto(sd, id, strlen(id), 0, adrresults->ai_addr, adrresults->ai_addrlen);
    if (snd < 0) {
        printf("ERROR: sendto() failed\n");
        return;
    }
}

int main(int argc, char * argv[]) {
    
    if (argc < 5) {
        printf("ERROR: invalid arguments\n");
        printf("USAGE: : ./a.out <rendezvous server address> <your peer ID> <contact peer ID> <message>\n");
        return EXIT_FAILURE;
    }
    
    char ownPeerID[80];
    strcpy(ownPeerID, argv[2]);
    
    char registerID[80] = "REGISTER ";
    
    strcat(registerID, ownPeerID);
    registerID[strlen(registerID)] = '\0';

    int sd;
    if((sd = socket( AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed\n");
        return EXIT_FAILURE;
    }
    sendMessageUDP(registerID, sd, argv[1]);
    //server.sin_family = AF_INET;
    //server.sin_addr.s_addr = htonl(INADDR_ANY);
    //server.sin_port = htons(12345);
    char peerID[80];
    char message[80];
    strcpy(peerID, argv[3]);
    strcpy(message, argv[4];)
    
    
    return EXIT_SUCCESS;
    
}