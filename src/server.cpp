#include <iostream>
#include <cerrno>
#include <cstring> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <unordered_map>
#include "server.h"


int create_server_socket() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; //Sets address family to IPv4
    server_addr.sin_port = htons(8080); //Sets port to listen on to 8080
    server_addr.sin_addr.s_addr = INADDR_ANY;   //Set IP to accept connections. INADOR_ANY = all available interfaces


    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to Bind: " << strerror(errno) << "\n";
        exit(1);
    }

    if (listen(sock_fd, BACKLOG) == -1) {
        std::cerr << "Listen Failed: " << strerror(errno) << "\n";
        exit(1);
    }

    return sock_fd;
}

int accept_connection(int sock_fd) {
    struct sockaddr_in client_addr;
    int client_size = sizeof(client_addr);

    int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_size);
    std::cout << "Client connected\n";

    return client_fd;
}

std::string receive_request(int client_fd) {
    char buf[100];
    int bytes_received = recv(client_fd, buf, 99, 0);


    buf[bytes_received] = '\0'; //Adds null terminator to buffer
    std::cout << "Client sent:\n" << buf << "\n";
    return std::string(buf, bytes_received);
}

//Function to Parse Http
std::unordered_map<std::string, std::string> parse(std::string request) {
    std::unordered_map<std::string, std::string> parsed_response;
    std::string line;
    std::istringstream request_stream(request);
    
    getline(request_stream, line);

    while (getline(request_stream, line)) {
        size_t split_location = line.find(':');
        if (split_location != std::string::npos) {
            std::string key = line.substr(0, split_location);
            std::string value = line.substr(split_location + 2);

            parsed_response[key] = value;
        }
    }
    // for (const auto& pair : parsed_response) {
    //     std::cout << "Key: " << pair.first << " Value: " << pair.second << "\n";
    // }
    return parsed_response;
}

void send_response(std::unordered_map<std::string, std::string> &parsed_resp, int client, std::string path) {
    std::string response = "";

    if (path == "/") {
        response = "HTTP/1.1 200 OK\r\n\r\n";
    } else if (path == "/user-agent" && parsed_resp.count("User-Agent") != 0) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " 
                                + std::to_string(parsed_resp["User-Agent"].length()) 
                                + "\r\n\r\n" 
                                + parsed_resp["User-Agent"];
    } else if (path.find("/echo/") != std::string::npos){
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " 
                                + std::to_string(path.length() - 6) 
                                + "\r\n\r\n" 
                                + path.substr(6);
    } else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    send(client, response.c_str(), response.size(), 0);
}
