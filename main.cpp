#include "manager.hpp"
#include "server.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    constexpr const char *open_message = R"(
StudentManager with zzuli.
  _________ __            .___             __       _____                                             
 /   _____//  |_ __ __  __| _/____   _____/  |_    /     \ _____    ____ _____     ____   ___________ 
 \_____  \\   __\  |  \/ __ |/ __ \ /    \   __\  /  \ /  \\__  \  /    \\__  \   / ___\_/ __ \_  __ \
 /        \|  | |  |  / /_/ \  ___/|   |  \  |   /    Y    \/ __ \|   |  \/ __ \_/ /_/  >  ___/|  | \/
/_______  /|__| |____/\____ |\___  >___|  /__|   \____|__  (____  /___|  (____  /\___  / \___  >__|   
        \/                 \/    \/     \/               \/     \/     \/     \//_____/      \/       
    )";

    std::cout << open_message << std::endl;

    init_manager();
    start_server(argv[1], argv[2]);

    process_client();
    exit_server();

    return 0;
}