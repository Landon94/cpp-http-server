#ifndef SERVER
#define SERVER

#include <cstring> 
#include <unordered_map>
#include <sstream>
#include <unistd.h>

constexpr int BACKLOG = 10;


int create_server_socket();
int accept_connection(int sock_fd);
std::string receive_request(int client_fd);
std::unordered_map<std::string, std::string> parse(std::string request);
void send_response(std::unordered_map<std::string, std::string> &parsed_resp, int client, std::string path);

#endif