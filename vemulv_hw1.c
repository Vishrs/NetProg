#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char * argv[]) {
    
    if (argc < 5) {
        printf("ERROR: invalid arguments\n");
        printf("USAGE: : ./a.out <rendezvous server address> <your peer ID> <contact peer ID> <message>\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
    
}