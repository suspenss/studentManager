#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
// local
#include "../include/client_manager.hpp"
#include "../include/client_server.hpp"
#include "../include/socket.hpp"

#define ANSI_MOVE_TO_START "\033[1G"

// local

namespace client_server {

    static tcp::SocketServer CLIENT {};

    void start_server(char *port, char *ipaddr) {
        CLIENT.init(port, ipaddr);
        CLIENT.connect_server();
        std::cout << "start_server complete!" << std::endl;
    }

    int server_socket() {
        return CLIENT.get_socket();
    }

    void process() {
        constexpr const char *open_message = R"(
StudentManager with zzuli.
  _________ __            .___             __       _____                                             
 /   _____//  |_ __ __  __| _/____   _____/  |_    /     \ _____    ____ _____     ____   ___________ 
 \_____  \\   __\  |  \/ __ |/ __ \ /    \   __\  /  \ /  \\__  \  /    \\__  \   / ___\_/ __ \_  __ \
 /        \|  | |  |  / /_/ \  ___/|   |  \  |   /    Y    \/ __ \|   |  \/ __ \_/ /_/  >  ___/|  | \/
/_______  /|__| |____/\____ |\___  >___|  /__|   \____|__  (____  /___|  (____  /\___  / \___  >__|   
        \/                 \/    \/     \/               \/     \/     \/     \//_____/      \/       
    )";
        constexpr const char *menu = R"(
-----------------------------------------------------
1: 添加学生信息 - [add]
2: 删除学生信息 - [delete]
3: 修改学生信息 - [edit]
4: 查找学生信息 - [find]
5: 展示学生信息 - [show]
6: 打印系统菜单 - [help]
0: 退出管理程序 - [exit]
-----------------------------------------------------
)";

        std::cout << open_message << '\n' << menu << std::endl;

        while (true) {
            std::cout << "StudentManager) ";
            std::string information;
            std::getline(std::cin, information);
            // std::cout << information << std::endl;

            if (information.find("add") != std::string::npos) {
                client_manager::add();
                std::cout << "add complete";
            } else if (information.find("delete") != std::string::npos) {
                client_manager::remove();
                std::cout << "remove complete";
            } else if (information.find("show") != std::string::npos) {
                client_manager::show();
                std::cout << "show complete";
            } else if (information.find("edit") != std::string::npos) {
                client_manager::modify();
                std::cout << "edit complete";
            } else if (information.find("find") != std::string::npos) {
                client_manager::search();
                std::cout << "find complete";
            } else if (information.find("help") != std::string::npos) {
                std::cout << menu;
            } else if (information.find("exit") != std::string::npos) {
                break;
            }
            std::cout << std::endl;
        }
    }

    void exit_server() {
        CLIENT.exit_socket();
        std::cout << "exit complete!";
    }
}    // namespace client_server

namespace client_server {
    auto Send(std::string_view send_msg) -> ssize_t {
        auto len = send_msg.size();
        std::cout << "SEND IS HERE " << len << std::endl;
        len = htonl(len);
        auto result = ::send(server_socket(), &len, sizeof(len), 0);
        if (result > 0) {
            return ::send(server_socket(), send_msg.data(), send_msg.size(), 0);
        } else {
            return result;
        }
    }

    auto Recv(std::string &recv_msg) -> ssize_t {
        size_t len;
        auto result = ::recv(server_socket(), &len, sizeof(len), 0);
        if (result > 0) {
            len = ntohl(len);
            recv_msg.assign(len + 1, '\0');
            return ::recv(server_socket(), &recv_msg[0], len, 0);
        } else {
            return result;
        }
    }
}    // namespace client_server