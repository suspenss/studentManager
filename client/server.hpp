#ifndef CLIENT_SERVER_HPP
#define CLIENT_SERVER_HPP

void start_server(char *port, char *ipaddr);

int server_socket();

void exit_server();

void process();

#endif    // CLIENT_SERVER_HPP
