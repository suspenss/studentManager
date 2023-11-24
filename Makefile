CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -MMD
LIBS = -lmysqlclient

SERVER_EXECUTABLE = server
CLIENT_EXECUTABLE = client

SRC_DIR = server
CLIENT_DIR = client
INCLUDE_DIR = include
BUILD_DIR = build

# 使用通配符获取源文件列表
SERVER_SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CLIENT_SRC_FILES = $(wildcard $(CLIENT_DIR)/*.cpp)
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)

# 生成对象文件列表，放在 build 目录下
SERVER_OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(notdir $(SERVER_SRC_FILES:.cpp=.o)))
CLIENT_OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(notdir $(CLIENT_SRC_FILES:.cpp=.o)))

DEP_FILES = $(SERVER_OBJ_FILES:.o=.d) $(CLIENT_OBJ_FILES:.o=.d)

IP_ADDRESS ?= 127.0.0.1
PORT ?= 9999

MYSQL_SOCKET ?= /var/run/mysqld/mysqld.sock
MYSQL_USER ?= root
MYSQL_PASSWORD ?= 1202

all: $(BUILD_DIR) $(BUILD_DIR)/$(SERVER_EXECUTABLE) $(BUILD_DIR)/$(CLIENT_EXECUTABLE)

$(BUILD_DIR)/$(SERVER_EXECUTABLE): $(SERVER_OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/$(CLIENT_EXECUTABLE): $(CLIENT_OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

-include $(DEP_FILES)

# 编译规则，将每个源文件编译为对应的对象文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_FILES)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

$(BUILD_DIR)/%.o: $(CLIENT_DIR)/%.cpp $(INCLUDE_FILES)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

server: start_mysql $(BUILD_DIR)/$(SERVER_EXECUTABLE)
	./$(BUILD_DIR)/$(SERVER_EXECUTABLE) $(PORT) $(IP_ADDRESS)

client: $(BUILD_DIR)/$(CLIENT_EXECUTABLE)
	./$(BUILD_DIR)/$(CLIENT_EXECUTABLE) $(PORT) $(IP_ADDRESS)

start_mysql:
	service mysql start

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all run_server run_client clean start_mysql rebuild
