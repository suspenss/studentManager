#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
// local header

namespace tcp {
    // socket服务器，封装了 socket 通信的 套接字，地址等参数
    // 以及根据 ip 和 port 初始化操作，bind, listen, accept 操作
    class SocketServer {
      private:
        sockaddr_in server_addr = {};
        int server_socket;

      public:
        SocketServer() {
            server_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (server_socket == -1) {
                // throw std::runtime_error("Failed to create socket");
                exit(0);
            }

            std::cout << "Socket server is online!\n";
        }
        // RAII
        ~SocketServer() {
            close(server_socket);
        }

        // 初始化，转换端口为数字，并将ip地址转换为网络序，获取套接字，设置套接字地址
        void init(char *arg_port, char *arg_ipaddress) {
            int port = atoi(arg_port);
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            inet_pton(server_socket, arg_ipaddress, &server_addr.sin_addr);
        }

        // 封装了的 bind 和 listen 操作
        // tcp/ip 协议，三次握手
        void bind_and_listen() {
            if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1) {
                std::cerr << "bind is fail " << std::endl;
                exit(0);
            }
            std::cout << "Socket bind complete!\n";

            if (listen(server_socket, 5) == -1) {
                std::cerr << "listen is fail " << std::endl;
                exit(0);
            }
            std::cout << "Socket listen complete!\n";
        }

        /// 当服务器接收到客户端的连接请求时，返回 true 否则会阻塞线程知道错误，退出，和接收到客户端的连接请求
        bool accept_server(SocketServer &client) {
            socklen_t clientAddressLength = sizeof(client.server_addr);

            client.server_socket = accept(server_socket, (struct sockaddr *)&client.server_addr, &clientAddressLength);

            if (client.server_socket == -1) {
                std::cerr << "accept is fail: " << std::endl;
                exit(0);
            }

            return true;
        }

        bool connect_server() {
            int ret = connect(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
            if (ret == -1) {
                std::cerr << "connect is fail: " << std::endl;
                exit(0);
            }
            return true;
        }

        int get_socket() const {
            return server_socket;
        }

        void exit_socket() {
            close(server_socket);
        }
    };
}    // namespace tcp