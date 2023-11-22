#ifndef MANAGER_SERVER_HPP
#define MANAGER_SERVER_HPP

extern thread_local int THREAD_SOCKET;

void start_server(char *port, char *ipaddr);

int server_socket();

void process_client();

#endif    // MANAGER_SERVER_HPP
