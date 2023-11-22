CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -MMD
LIBS = -lmysqlclient

EXECUTABLE = main
SRC_DIR = server
INCLUDE_DIR = include
BUILD_DIR = build

# 使用通配符获取源文件列表
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
# 生成对象文件列表，放在 build 目录下
OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_FILES:.cpp=.o)))
DEP_FILES = $(OBJ_FILES:.o=.d)

IP_ADDRESS ?= 127.0.0.1
PORT ?= 9808

MYSQL_SOCKET ?= /var/run/mysqld/mysqld.sock
MYSQL_USER ?= root
MYSQL_PASSWORD ?= 1202

all: $(BUILD_DIR) $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

-include $(DEP_FILES)

# 编译规则，将每个源文件编译为对应的对象文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

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
