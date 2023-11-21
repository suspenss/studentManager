#include "manager.hpp"
#include "server.hpp"

int main(int argc, char *argv[]) {
    init_manager();
    start_server(argv[1], argv[2]);

    return 0;
}