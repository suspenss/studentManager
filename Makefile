CC := g++
CFLAGS := -std=c++17 -Wall -Wextra
INCLUDES := -Iinclude
LIBS := -lmysqlclient
SRC_DIR := .
BUILD_DIR := build

# List of source files for client and server
CLIENT_SRC := $(wildcard client/*.cpp)
SERVER_SRC := $(wildcard server/*.cpp)

# List of object files for client and server
CLIENT_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CLIENT_SRC))
SERVER_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SERVER_SRC))

IP_ADDRESS ?= 127.0.0.1
PORT ?= 9998

# Targets
all: client server

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $(BUILD_DIR)/$@ 

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $(BUILD_DIR)/$@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: run_client
run_client: client
	./$(BUILD_DIR)/client $(PORT) $(IP_ADDRESS)  

.PHONY: run_server
run_server: server
	./$(BUILD_DIR)/server $(PORT) $(IP_ADDRESS)


