#include <iostream>
#include <cerrno>
#include <cstring> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BACKLOG 10

int main(int argc, int *argv[]){

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "Failed to create socket\n";
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; //Sets address family to IPv4
    server_addr.sin_port = htons(8080); //Sets port to listen on to 8080
    server_addr.sin_addr.s_addr = INADDR_ANY;   //Set IP to accept connections. INADOR_ANY = all available interfaces


    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to Bind: " << strerror(errno) << "\n";
    }

    if (listen(sock_fd, BACKLOG) == -1) {
        std::cerr << "Listen Failed: " << strerror(errno) << "\n";
    }

    return 0;
}