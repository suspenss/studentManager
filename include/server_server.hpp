#pragma once
#include <string_view>
#include <sys/types.h>

namespace server {
    // 运行服务器
    void start_server(char *port, char *ipaddr);

    // 监听，分配，处理客户端
    void process_client();

    // 返回服务器套接字
    int server_socket();

    // 退出服务器，关闭套接字
    void exit_server();

    auto Send(std::string_view) -> ssize_t;

    auto Recv(std::string &) -> ssize_t;
}    // namespace server

extern thread_local int THREAD_SOCKET;
