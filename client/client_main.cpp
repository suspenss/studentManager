#include <iostream>
/// local header
#include "client_server.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Program have " << argc << " arguments." << std::endl;

    start_server(argv[1], argv[2]);
    process();
    exit_server();

    std::cout << "Complete!" << std::endl;
}