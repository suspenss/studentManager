// server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP

class Server;

void start_server(char *port, char *ipaddr);

int server_sock_fd();

#endif    // SERVER_HPP
