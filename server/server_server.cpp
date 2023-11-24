#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
// local
#include "../include/server_manager.hpp"
#include "../include/server_server.hpp"
#include "../include/socket.hpp"

/// 该变量会在每个线程中都存在
thread_local int THREAD_SOCKET;

namespace server {
    static tcp::SocketServer STUDENT_MANAGER_SERVER {};

    /// 具体的和客户端进行socket 通信的函数，接收客户端的 socket 字节流，并执行相应功能
    void handle_client(int socket) {
        std::cout << "SOCKET" << socket << std::endl;
        THREAD_SOCKET = socket;
        while (true) {
            std::string buffer;
            if (server::Recv(buffer) > 0) {
                std::cout << "RECV: " << buffer << '\n';
                // 处理从客户端读到的字符串
                // todo 添加相关提示信息
                if (buffer.find("add") != std::string::npos) {
                    std::cout << "add mode\n";
                    server_manager::add();
                    std::cout << "add complete!" << std::endl;
                } else if (buffer.find("delete") != std::string::npos) {
                    server_manager::remove();
                } else if (buffer.find("show") != std::string::npos) {
                    server_manager::show();
                } else if (buffer.find("edit") != std::string::npos) {
                    server_manager::modify();
                } else if (buffer.find("find") != std::string::npos) {
                    server_manager::search();
                } else if (buffer.find("exit") != std::string::npos) {
                    std::cout << "客户端" << std::this_thread::get_id() << "已经退出\n";
                    break;
                }
                /// 处理客户端的请求，并展示提示性的信息，调用相关函数
            }
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
        tcp::SocketServer client_address;
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

namespace server {
    auto Send(std::string_view send_msg) -> ssize_t {
        auto len = send_msg.size();
        len = htonl(len);
        auto result = ::send(THREAD_SOCKET, &len, sizeof(len), 0);
        if (result > 0) {
            return ::send(THREAD_SOCKET, send_msg.data(), send_msg.size(), 0);
        } else {
            return result;
        }
    }

    auto Recv(std::string &recv_msg) -> ssize_t {
        size_t len;
        auto result = ::recv(THREAD_SOCKET, &len, sizeof(len), 0);
        if (result > 0) {
            len = ntohl(len);
            // std::cout << "RECV IS HERE " << len << std::endl;
            recv_msg.assign(len + 1, '\0');
            return ::recv(THREAD_SOCKET, &recv_msg[0], len, 0);
        } else {
            return result;
        }
    }

}    // namespace server