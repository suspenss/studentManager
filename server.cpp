#include "server.hpp"
#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>

static SocketServer STUDENT_MANAGER_SERVER {};

void start_server(char *port, char *ipaddr) {
    STUDENT_MANAGER_SERVER.init(port, ipaddr);
}

int server_socket() {
    return STUDENT_MANAGER_SERVER.get_socket();
}

void process_client() {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    while (true) {
        int clientSocket = accept(server_socket(), (struct sockaddr *)&clientAddress, &clientAddressLength);

        if (clientSocket == -1) {
            std::cerr << "accept is fail: " << std::endl;
            exit(0);
        }
        std::cout << "有新的客户端连接: " << clientSocket << std::endl;

        constexpr const char *msg = R"(
/********************/
1、显示学生信息
2、添加学生信息
3、删除学生信息
4、修改学生信息
5、查找学生信息
0、退出
/*********************/
)";

        write(clientSocket, msg, std::strlen(msg));
    }
}