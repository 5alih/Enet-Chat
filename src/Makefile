CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lenet -pthread

# Output binaries
SERVER_BIN = server
CLIENT_BIN = client

# Source files
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

# Default target - builds everything but only runs server
all: build run_server

# Build both server and client
build: $(SERVER_BIN) $(CLIENT_BIN)

# Build and run the server
run_server: $(SERVER_BIN)
	./$(SERVER_BIN)

# Build server
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN) $(LDFLAGS)

# Build client
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN) $(LDFLAGS)

# Clean built files
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

# Only build both binaries
build_only: $(SERVER_BIN) $(CLIENT_BIN)

# Run existing server binary without rebuilding
start_server:
	./$(SERVER_BIN)

.PHONY: all build run_server clean build_only start_server