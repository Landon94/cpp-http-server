#include <iostream>
#include <cerrno>
#include <cstring> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>


#define BACKLOG 10

std::string messageStr(char* buf, int size) {
    return std::string(buf, size);
}


int main(int argc, char **argv){

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


    struct sockaddr_in client_addr;
    int client_size = sizeof(client_addr);

    int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_size);
    std::cout << "Client connected\n";



    char buf[100];
    int bytes_recieved = recv(client_fd, buf, 99, 0);


    buf[bytes_recieved] = '\0'; //Adds null terminator to buffer
    std::cout << "Bytes received: " << bytes_recieved<< "\n";
    std::cout << "Client sent:\n" << buf << "\n";

    std::string request = messageStr(buf, 100);
    std::istringstream request_stream(request);
    std::string method, path, version;
    request_stream >> method >> path >> version;

    std::cout << "Path: " << path << "\n";

    if (path == "/") {
        std::string aprove_res = "HTTP/1.1 200 OK\r\n\r\n";
        send(client_fd, aprove_res.c_str(), aprove_res.size(), 0);
    } else {
        std::string aprove_res = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(client_fd, aprove_res.c_str(), aprove_res.size(), 0);
    }


    close(client_fd);
    close(sock_fd);
    
    return 0;
}