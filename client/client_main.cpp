#include <iostream>
/// local header
#include "../include/client_server.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Program have " << argc << " arguments." << std::endl;

    client_server::start_server(argv[1], argv[2]);
    client_server::process();
    client_server::exit_server();

    std::cout << "Complete!" << std::endl;
}