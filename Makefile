CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -MMD
LIBS = -lmysqlclient

EXECUTABLE = main
SRC_FILES = main.cpp server.cpp manager.cpp
OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(SRC_FILES:.cpp=.o))
DEP_FILES = $(OBJ_FILES:.o=.d)
BUILD_DIR = build

IP_ADDRESS ?= 127.0.0.1
PORT ?= 9808

MYSQL_SOCKET ?= /var/run/mysqld/mysqld.sock
MYSQL_USER ?= your_mysql_user
MYSQL_PASSWORD ?= your_mysql_password

all: $(BUILD_DIR) $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

-include $(DEP_FILES)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: start_mysql $(BUILD_DIR)/$(EXECUTABLE)
	./$(BUILD_DIR)/$(EXECUTABLE) $(PORT) $(IP_ADDRESS)

start_mysql:
	service mysql start

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all   

.PHONY: all run clean start_mysql rebuild
