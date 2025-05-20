#include <signal.h>
#include <iostream>
#include "threadpool.h"
#include <sys/select.h>
#include "server.h"

static bool stop_server = true;

void intHandler(int) {
    std::cout << "\nShutting Down Sever\n";
    stop_server = false;
}

int main(int argc, char **argv){
    signal(SIGINT, intHandler);
    int sock_fd = create_server_socket();
    ThreadPool pool(4);

    
    ///int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *utimeout);
//     FD_ZERO - Clear an fd_set
// FD_ISSET - Check if a descriptor is in an fd_set
// FD_SET - Add a descriptor to an fd_set
// FD_CLR - Remove a descriptor from an fd_set
    while(stop_server) {

        fd_set readfds; //set of socket/file descriptors
        FD_ZERO(&readfds); //clear the socket set
        FD_SET(sock_fd, &readfds); //clear the socket set
        
        struct timeval timeout;
        timeout.tv_sec = 1; //seconds
        timeout.tv_usec = 0; //microseconds

        //waits for activity on the sockets; timout is 1 second
        int activity = select(sock_fd + 1 , &readfds , NULL , NULL , &timeout);
        
        //Signal interruppted
        if (activity == -1 && errno == EINTR) {
            if (!stop_server) break;
            continue;
        }

        if (activity == -1) {
            perror("select()");
            exit(1);
        }

        if (FD_ISSET(sock_fd, &readfds)) {
            int client_fd = accept_connection(sock_fd);
            pool.enqueue( [client_fd] {
                std::string request = receive_request(client_fd);
                std::istringstream request_stream(request);
                std::string method, path, version;
                request_stream >> method >> path >> version;

                // std::cout << "Path: " << path << "\n"; 

                std::unordered_map<std::string, std::string> parsed_resp = parse(request); 


                send_response(parsed_resp, client_fd, path);

                close(client_fd);
            });
        }
    }

    close(sock_fd);
    return 0;
}