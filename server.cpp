#include "server.hpp"
#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>

static SocketServer STUDENT_MANAGER_SERVER {};

thread_local int THREAD_SOCKET;

void handle_client(int socket) {
    std::string buffer(100, '\0');
    while (recv(socket, &buffer[0], buffer.size(), 0) > 0) {
        /// 处理客户端的请求，并展示提示性的信息，调用相关函数
    }
}

void start_server(char *port, char *ipaddr) {
    STUDENT_MANAGER_SERVER.init(port, ipaddr);
}

int server_socket() {
    return STUDENT_MANAGER_SERVER.get_socket();
}

void process_client() {
    SocketServer client_address;
    while (true) {
        if (STUDENT_MANAGER_SERVER.accept_server(client_address)) {
            std::cout << "有新的客户端连接: " << client_address.get_socket() << std::endl;
            std::thread read_thread(handle_client, client_address.get_socket());
            read_thread.detach();
        }

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

        write(client_address.get_socket(), msg, std::strlen(msg));
    }
}
