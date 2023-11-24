#include "../include/socket.hpp"
#include <iostream>
#include <mutex>
#include <thread>

#define ANSI_MOVE_TO_START "\033[1G"

// local
std::mutex input_mutex;

static network::SocketServer CLIENT {};

void client_reader(int socket_fd) {
    while (true) {
        std::string buffer(1024, '\0');

        size_t byte_received = recv(socket_fd, &buffer[0], buffer.size(), 0);
        if (byte_received > 0) {
            std::cout << buffer << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } else if (byte_received == 0) {
            std::cout << "client:  service disconnected\n " << std::endl;
            break;
        } else {
            std::cout << "error in client raeder" << std::endl;
            break;
        }
    }
}

void start_server(char *port, char *ipaddr) {
    CLIENT.init(port, ipaddr);
    CLIENT.connect_server();
    std::cout << "start_server complete!" << std::endl;
}

int server_socket() {
    return CLIENT.get_socket();
}

void process() {
    std::thread reader_thread(client_reader, CLIENT.get_socket());

    while (true) {
        std::lock_guard<std::mutex> lock(input_mutex);

        std::string information;
        std::cout << ANSI_MOVE_TO_START << "StudentManager) ";
        std::getline(std::cin, information);
        std::cout << information << '\n';

        if (send(CLIENT.get_socket(), information.c_str(), information.size(), 0) < 0) {
            std::cerr << "Error writing to socket" << std::endl;
            break;
        }
        std::cout << std::endl;
    }

    reader_thread.detach();
}

void exit_server() {
    CLIENT.exit_socket();
}
