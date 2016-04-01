#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

using std::string;
#define PORT 80

//=====================================Helper functions===============================
bool icompare_pred(unsigned char a, unsigned char b)
{
    return std::tolower(a) == std::tolower(b);
}

bool icompare(std::string const& a, std::string const& b)
{
    if (a.length()==b.length()) {
        return std::equal(b.begin(), b.end(),
                           a.begin(), icompare_pred);
    }
    else {
        return false;
    }
}
//===================================================================================


void parse_url(string &url, string &host, string &port, string &path) {
    if (icompare(url.substr(0,4), "http")) {
        int port_ind = url.find(":", 5);
        int path_ind = url.find("/", 7);
        bool path_exists = true;
        if (path_ind == string::npos) {
            path_ind = url.size();
        }
        if (port_ind == string::npos) {
            port = "80";
            port_ind = path_ind;
        } else {
            port = url.substr(port_ind + 1, path_ind - port_ind - 1);
        }
        
        host = url.substr(7, port_ind - 7);
        path = url.substr(path_ind, url.size() - path_ind);
    }
    else {
        printf("Invalid url, include http://\n");
        return;
    }
}

connect_to_server(int &sockfd, char* host, char* port) {
    struct addrinfo ai_hints;
    struct addrinfo* ai_results;
    
    memset(&ai_hints, 0, sizeof(ai_hints));
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;   
    if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("sockfd()");
        return;
    }
    int ret = getaddrinfo(host, port, &ai_hints, &ai_results);
    if (ret < 0) {
        perror("getaddrinfo()");
        return;
    }
    
    bool connected = false;
    int i;
	for (i = ai_results;i != NULL;i = j->ai_next) {
		ret = connect(sockfd, i->ai_addr, i->ai_addrlen);
		if (ret == 0) {
			conntected = true;
			break;
		}
	}
    
    
}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Invalid arguments\n");
        return EXIT_FAILURE;
    }
    
    string input = argv[1];
    string host, port, path;
    char* c_host = host.c_str();
    char* c_port = port.c_str();

    parse_url(input, host, port, path);
    //std::cout << input << " " << host << " " << port << " " << path << std::endl;
    int sockfd;
    connect_to_server(sockfd, c_host, c_port);
    
    
    return EXIT_SUCCESS;
}