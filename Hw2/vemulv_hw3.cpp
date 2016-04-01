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

void parse_url(string &url, string &host, string &port, string &path) {
    if (icompare(url.substr(0,4), "http"))
        int host_ind = url.find(":", 5);
    else {
        printf("Invalid url, include http://\n");
        return;
    }
}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Invalid arguments\n");
        return EXIT_FAILURE;
    }
    
    string input = argv[1];
    string host, port, path;
    parse_url(input, host, port, path);
    
    return EXIT_SUCCESS;
}