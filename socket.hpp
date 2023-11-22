// server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <iostream>
// local header

class SocketServer {
  private:
    sockaddr_in server;
    int server_socket;

  public:
    SocketServer() {}

    void init(char *arg_port, char *arg_ipaddress) {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (server_socket == -1) {
            std::cerr << "socket is fail " << std::endl;
            exit(0);
        }

        int port = atoi(arg_port);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(arg_ipaddress);

        if (bind(server_socket, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1) {
            std::cerr << "bind is fail " << std::endl;
            exit(0);
        }

        if (listen(server_socket, 5) == -1) {
            std::cerr << "listen is fail " << std::endl;
            exit(0);
        }
    }

    int get_socket() {
        return server_socket;
    }
};

#endif    // SERVER_HPP
