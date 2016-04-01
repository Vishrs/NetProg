/*
    Vishal Vemula
    HW1
*/

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
#define BUFFER_SIZE 80

void sendMessageUDP(char *id, int sd, char *servaddr, const char* port) {
    struct addrinfo adrinfo;
    struct addrinfo *adrresults;
    
    memset(&adrinfo, 0, sizeof(adrinfo));
    adrinfo.ai_family = AF_INET6;
    adrinfo.ai_socktype = SOCK_DGRAM;
    adrinfo.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
    
    int rend_info = getaddrinfo(servaddr, port, &adrinfo, &adrresults);
    
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

void recvFromServer(int sd, char* buf) {
    
    struct sockaddr_in6 client;
    socklen_t client_len = sizeof(client);
    
    int rd = recvfrom(sd, buf, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len);
    if (rd < 0) {
        printf("ERROR: recvfrom() failed\n");
        return;
    }
     
    buf[rd >= BUFFER_SIZE ? BUFFER_SIZE - 1 : rd] = '\0';
    
    char host[BUFFER_SIZE], service[BUFFER_SIZE];
    int r = getnameinfo((struct sockaddr *)&client, client_len, host, sizeof(host),
                        service, sizeof(service), 0 | NI_NUMERICHOST);
                  
    if (r < 0) {
        printf("ERROR: getnameinfo() failed\n");
        return;
    }

}

 void recvFromServerInf(char *mssg, int sd) {
     char host[80], service[80];
     struct sockaddr_in6 client;
     socklen_t client_len = sizeof(client);
     int r, rd;
     while(1) {
         r = recvfrom(sd, mssg, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len);
         if (r < 0) {
             printf("ERROR: recvfrom() failed\n");
             return;
         }
         
         mssg[r >= BUFFER_SIZE ? BUFFER_SIZE - 1 : r] = '\0';
         
         rd = getnameinfo((struct sockaddr *)&client, client_len, host, sizeof(host), 
                           service, sizeof(service), 0 | NI_NUMERICHOST);
         
         if (rd < 0) {
             printf("ERROR: getnameinfo() failed\n");
             return;
         } else {
             printf("received from %s (%s): %s\n", host, service, mssg);
       
         }
         
         
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
    sendMessageUDP(registerID, sd, argv[1], PORT);
    
    //do some argument parsing
    char peerID[BUFFER_SIZE];
    char contactID[BUFFER_SIZE] = "GET_ADDR ";
    char message[BUFFER_SIZE];
    strcpy(peerID, argv[3]);
    strcat(contactID, peerID);
    contactID[strlen(contactID)] = '\0';
    strcpy(message, argv[4]);
    
    sendMessageUDP(contactID, sd, argv[1], PORT);
    char buffer[BUFFER_SIZE];
    
    recvFromServer(sd, buffer);

    char address[80] = "", port[80];
 
    if (strcmp(buffer, "NOT FOUND\0") != 0) {
        char *index = strchr(buffer, ' ');
        strncpy(address, buffer, index - buffer);
        strcpy(port, &buffer[index - buffer + 1]);
        sendMessageUDP(argv[4], sd, address, port);
    } else printf("not found\n");
  
    char mssg[80] = "";
    
    recvFromServerInf(mssg, sd);
    close(sd);
    return EXIT_SUCCESS;
    
}