CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

EXECUTABLE = client
SRC_FILES = client.cpp
BUILD_DIR = build

IP_ADDRESS ?= 127.0.0.1
PORT ?= 9808

all: $(BUILD_DIR) $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(addprefix $(BUILD_DIR)/, $(SRC_FILES:.cpp=.o))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(BUILD_DIR)/$(EXECUTABLE)
	./$< $(PORT) $(IP_ADDRESS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
