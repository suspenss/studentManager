#ifndef MANAGER_SERVER_HPP
#define MANAGER_SERVER_HPP

extern thread_local int THREAD_SOCKET;

void start_server(char *port, char *ipaddr);

void process_client();

int server_socket();

void exit_server();

#endif    // MANAGER_SERVER_HPP
