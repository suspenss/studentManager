#include <arpa/inet.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void *client_reader(int socket_fd) {
    while (true) {
        std::string buffer(1024, '\0');    // Initialize buffer with size 1024

        size_t byte_received = read(socket_fd, &buffer[0], sizeof(buffer));
        if (byte_received > 0) {
            std::cout << buffer << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } else if (byte_received == 0) {
            std::cout << "client: " << std::this_thread::get_id() << " disconnected\n" << std::endl;
        } else {
            std::cout << "error in thread: " << std::this_thread::get_id() << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    std::cout << "Program have " << argc << " arguments." << std::endl;

    int socket_fd, port;
    struct sockaddr_in client;

    // verfing ...
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        std::cerr << "socket is fail: " << std::endl;
        exit(EXIT_FAILURE);
    }

    // connecting ...
    port = atoi(argv[1]);
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    inet_pton(AF_INET, argv[2], &client.sin_addr);

    if (connect(socket_fd, (struct sockaddr *)&client, sizeof(sockaddr_in)) == -1) {
        std::cerr << "connect is fail" << std::endl;
        exit(0);
    }

    std::cout << port << ' ' << "connect";

    std::thread reader_thread(client_reader, socket_fd);

    while (true) {
        std::string information;
        std::getline(std::cin, information);

        // Add proper error handling for write
        if (write(socket_fd, information.c_str(), information.size()) < 0) {
            std::cerr << "Error writing to socket" << std::endl;
            break;
        }
    }

    reader_thread.join();
    close(socket_fd);

    std::cout << "Complete ..." << std::endl;
}