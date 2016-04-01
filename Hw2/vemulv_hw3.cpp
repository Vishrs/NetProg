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
const string method = "GET ";
const string version = " HTTP/1.1";
const string user_agent = "User-Agent: vemulv-netprog-hw3/1.0\r\n\r\n";

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
        if (path_ind == -1) {
            path_ind = url.size();
        }
        if (port_ind == -1) {
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

void connect_to_server(int &sockfd, string host, string port) {
    struct addrinfo ai_hints;
    struct addrinfo* ai_results;
    
    memset(&ai_hints, 0, sizeof(ai_hints));
	ai_hints.ai_family = AF_INET6;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;   
    if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("sockfd()");
        return;
    }
    std::cout << host << " " << port << std::endl;
    int ret = getaddrinfo(host.c_str(), port.c_str(), &ai_hints, &ai_results);
    if (ret < 0) {
        perror("getaddrinfo()");
        return;
    }
    
    bool connected = false;
    struct addrinfo* i;
	for (i = ai_results;i != NULL;i = i->ai_next) {
		ret = connect(sockfd, i->ai_addr, i->ai_addrlen);
		if (ret == 0) {
			connected = true;
			break;
		}
	}
	if (connected == false) {
	    printf("connection failed\n");
	    return;
	}
    
    
}

void send_mssgs(int sockfd, string req, string head) {
    string mssg = req + head + user_agent;
    const void *str_data = mssg.c_str();
    uint8_t *bytes = (uint8_t *)str_data;
    size_t size = mssg.size();
    int ret;
    while (size > 0) {
        if ((ret = send(sockfd, bytes, size, 0)) < 0) {
            perror("send()");
            return;
        } else {
            size -= ret;
			bytes += ret;
        }
        
    }
}

void recv_message(int sockfd) {
    char buf[BUFSIZ+1];
    memset(buf, 0, sizeof(buf));
    int htmlstart = 0;
    char * htmlcontent;
    int tmpres;
    bool first = true;
    while((tmpres = recv(sockfd, buf, BUFSIZ, 0)) > 0){
        if (first == true) {
             fwrite(buf , tmpres , 1  , stdout);
             first = false;
        }
        if(htmlstart == 0) {
         
            htmlcontent = strstr(buf, "\r\n\r\n");
            if(htmlcontent != NULL) {
                htmlstart = 1;
                htmlcontent += 4;
            }
        } else {
          htmlcontent = buf;
        }
        if(htmlstart){
          fprintf(stdout, "%s", htmlcontent);
        }
        memset(buf, 0, tmpres);
    }
    
  if(tmpres < 0)
  {
    perror("Error receiving data");
  }


  close(sockfd);
  return;
}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Invalid arguments\n");
        return EXIT_FAILURE;
    }
    
    string input = argv[1];
    string host, port, path;
    //char* c_host = (char*) host.c_str();
    //char* c_port = (char*) port.c_str();
    
    parse_url(input, host, port, path);
    //std::cout << input << " " << c_host + 1<< " " << c_port << " " << path << std::endl;
    int sockfd;
    connect_to_server(sockfd, host, port);

    string request = method + path + version + "\r\n";
    string header = "Host: " + host + ":" + port + "\r\n";
    send_mssgs(sockfd, request, header);
    recv_message(sockfd);
    
    return EXIT_SUCCESS;
}