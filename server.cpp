#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
// local header
#include "server.hpp"

class Server {
  private:
    sockaddr_in server;
    int sock_fd;

    Server() {}

  public:
    static Server &getInstance() {
        static Server instance {};
        return instance;
    }

    void init(char *arg_port, char *arg_ipaddress) {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (sock_fd == -1) {
            std::cerr << "socket is fail " << std::endl;
            exit(0);
        }

        int port = atoi(arg_port);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(arg_ipaddress);

        if (bind(sock_fd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1) {
            std::cerr << "bind is fail " << std::endl;
            exit(0);
        }

        if (listen(sock_fd, 5) == -1) {
            std::cerr << "listen is fail " << std::endl;
            exit(0);
        }
    }

    int get_fd() {
        return sock_fd;
    }
};

static Server &SERVER = Server::getInstance();

void start_server(char *port, char *ipaddr) {
    SERVER.init(port, ipaddr);
}

int server_sock_fd() {
    return SERVER.get_fd();
}
