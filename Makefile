CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

# server: src/server.cpp src/server.h
# 	$(CXX) $(CXXFLAGS) -c src/server.cpp src/server.h

# threadpool: src/threadpool.cpp src/threadpool.h
# 	$(CXX) $(CXXFLAGS) -c src/threadpool.cpp src/threadpool.h

main: src/main.cpp src/server.cpp src/threadpool.cpp
	$(CXX) $(CXXFLAGS) -o main src/main.cpp src/server.cpp src/threadpool.cpp -pthread

clean:
	rm -f main
