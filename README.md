# cpp-http-server

## ⭐Highlights
- Multithreaded: can handle multiple connectsion concurrenlty
- Graceful shutdown: Ctrl-C will gracefull shutdown server and threapool
- Responses: Responds to http get requests

## Overview
This project is simple HTTP server built in C++. I created this project to expand my knowledge of sockets, multithreading, and HTTP communication. 
The server listens on port '8080' and supports a few get requests.


## How to Run
```bash
make
./cpp-http-server
```
The server will start and listen on http://localhost:8080

Press Ctrl+C to stop the server.
### Examples
```shell
curl http://localhost:8080/

curl http://localhost:8080/echo/hello

curl http://localhost:8080/user-agent
```
## Resources
These sources provided helpul information for implementation of various parts of the project

https://www.binarytides.com/multiple-socket-connections-fdset-select-linux/

https://eli.thegreenplace.net/2017/concurrent-servers-part-2-threads/

https://linux.die.net/man/2/select_tut
