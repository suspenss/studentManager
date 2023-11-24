#pragma once
#include <cstddef>
#include <string_view>
#include <sys/types.h>

namespace client_server {
    void start_server(char *port, char *ipaddr);

    int server_socket();

    void exit_server();

    void process();

    auto Send(std::string_view) -> ssize_t;

    auto Recv(std::string &) -> ssize_t;
}    // namespace client_server