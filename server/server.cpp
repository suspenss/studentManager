#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
// local
#include "../include/manager.hpp"
#include "../include/server.hpp"
#include "../include/socket.hpp"

/// 该变量会在每个线程中都存在
thread_local int THREAD_SOCKET;

namespace server {
    static network::SocketServer STUDENT_MANAGER_SERVER {};

    /// 具体的和客户端进行socket 通信的函数，接收客户端的 socket 字节流，并执行相应功能
    void handle_client(int socket) {
        THREAD_SOCKET = socket;
        constexpr const char *msg = R"(
-----------------------------------------------------
1: 显示学生信息
2: 添加学生信息
3: 删除学生信息
4: 修改学生信息
5: 查找学生信息
0: 退出
-----------------------------------------------------
)";
        send(THREAD_SOCKET, msg, std::strlen(msg), 0);
        std::string buffer(200, '\0');
        while (recv(socket, &buffer[0], buffer.size(), 0) > 0) {
            std::cout << buffer << '\n';
            // 处理从客户端读到的字符串
            // todo 添加相关提示信息
            if (buffer.find("add") != -1) {
                manager::add();
            } else if (buffer.find("delete") != -1) {
                manager::remove();
            } else if (buffer.find("show") != -1) {
                manager::show();
            } else if (buffer.find("edit") != -1) {
                manager::modify();
            } else if (buffer.find("find") != -1) {
                manager::search();
            } else {
                // todo
                continue;
            }
            /// 处理客户端的请求，并展示提示性的信息，调用相关函数
        }
    }

    // 初始化socket 服务器，对于服务端来说，执行init之外还要进行 bind 和 listen 操作
    void start_server(char *port, char *ipaddr) {
        STUDENT_MANAGER_SERVER.init(port, ipaddr);
        STUDENT_MANAGER_SERVER.bind_and_listen();
        std::cout << "start_server complete!" << std::endl;
    }

    // 返回服务器的套接字
    int server_socket() {
        return STUDENT_MANAGER_SERVER.get_socket();
    }

    // 执行处理调度服务端与客户端通信的主要程序
    // 其主要功能是通过循环去监听是否有客户端连接，若有
    // 则新建一个线程分配给服务端，并将msg打印给客户端。
    void process_client() {
        network::SocketServer client_address;
        while (true) {
            if (STUDENT_MANAGER_SERVER.accept_server(client_address)) {
                std::cout << "有新的客户端连接: " << client_address.get_socket() << std::endl;
                std::thread read_thread(handle_client, client_address.get_socket());
                read_thread.detach();
            }
        }
    }

    // 关闭套接字
    void exit_server() {
        STUDENT_MANAGER_SERVER.exit_socket();
    }
}    // namespace server