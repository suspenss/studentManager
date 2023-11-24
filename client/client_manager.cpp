#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <vector>

// local
#include "../include/client_server.hpp"
#include "../include/student.hpp"

namespace client_manager {

    void send_operation(std::string_view operation) {
        client_server::Send(operation);
        // send(client_server::server_socket(), operation.data(), operation.size(), 0);
        std::cout << "SEND: " << operation << '\n';
    }

    bool handle_state() {
        std::string state;
        // recv(client_server::server_socket(), &state[0], state.size(), 0);
        client_server::Recv(state);
        return (state.find("true") != std::string::npos);
    }

    void add() {
        send_operation("add");

        std::cout << "添加学生信息\n";

        std::vector<std::string> send_msg_item {"学生姓名", "学生学号", "学生性别，[男/女]", "学生年龄",
            "学生成绩：数学成绩", "学生成绩：语文成绩", "学生成绩：英语成绩"};

        std::vector<std::string> student_info_vec;
        for (auto &item : send_msg_item) {
            std::string send_msg {"请输入信息：" + item};
            std::cout << send_msg << "\nadd)";
            std::string recv_msg;

            std::getline(std::cin, recv_msg);
            std::cout << recv_msg << '\n';
            student_info_vec.emplace_back(recv_msg);
        }

        std::string confirm_info;
        std::cout << "确认添加吗？[yes/no]\n";
        std::getline(std::cin, confirm_info);

        if (confirm_info.find("yes") != std::string::npos or confirm_info.find("no") == std::string::npos) {
            manager::Student s;
            s.name = student_info_vec[0];
            s.number = student_info_vec[1];
            s.gender = student_info_vec[2];
            s.age = std::stoi(student_info_vec[3]);
            s.math = std::stoi(student_info_vec[4]);
            s.english = std::stoi(student_info_vec[5]);
            s.chinese = std::stoi(student_info_vec[6]);

            std::string state = "true";
            // send(client_server::server_socket(), state.c_str(), state.size(), 0);
            client_server::Send(state);

            std::string send_info = manager::Student::serializeStudent(s);
            client_server::Send(send_info);
            // send(client_server::server_socket(), send_info.c_str(), send_info.size(), 0);
            std::cout << std::endl;

            if (handle_state()) {
                std::cout << "服务器操作成功 " << s.name << " 已添加到服务器\n";
            } else {
                std::cout << "服务器操作失败！\n";
            }
        } else {
            std::string state = "false";
            // send(client_server::server_socket(), state.c_str(), state.size(), 0);
            client_server::Send(state);
        }
    }

    void remove() {
        send_operation("delete");
        std::cout << "删除学生\n请输入要删除的学生的名字\ndelete)";

        std::string delete_name;
        std::getline(std::cin, delete_name);
        client_server::Send(delete_name);
        // send(client_server::server_socket(), delete_name.c_str(), delete_name.size(), 0);

        if (handle_state()) {
            std::cout << "服务器操作成功 " << delete_name << " 已经从服务器删除\n";
        } else {
            std::cout << "服务器操作失败！\n";
        }
    }

    void show() {
        send_operation("show");

        std::cout << "展示所有学生信息\n";

        if (handle_state()) {
            std::cout << "服务器操作成功，下面是所有的学生信息：\n";
        } else {
            std::cout << "服务器操作失败！\n";
        }

        std::string info;
        client_server::Recv(info);
        // recv(client_server::server_socket(), &info[0], info.size(), 0);
        std::cout << info << '\n';
    }

    void search() {
        send_operation("find");

        std::string send_msg {"查询学生: 请输入要查询的学生姓名\nsearch)"};
        std::cout << send_msg;
        std::string search_name;
        std::getline(std::cin, search_name);
        std::cout << search_name << std::endl;

        client_server::Send(search_name);
        // send(client_server::server_socket(), search_name.c_str(), search_name.size(), 0);

        if (handle_state()) {
            std::cout << "查找成功！\n";
        } else {
            std::cout << "查找失败！\n";
        }

        std::string info;
        client_server::Recv(info);
        // recv(client_server::server_socket(), &info[0], info.size(), 0);

        std::cout << info << std::endl;
    }

    void modify() {
        send_operation("edit");

        std::cout << "修改学生信息：请输入要选择修改的学生学号\nedit student)";
        std::string edit_number;
        std::getline(std::cin, edit_number);
        // 发送要修改的学生姓名
        client_server::Send(edit_number);

        if (handle_state()) {
            std::string recv_msg;
            client_server::Recv(recv_msg);
            std::cout << "查找成功，已经找到你要修改的对象\n" << recv_msg << "\nedit student)";
            constexpr const char *menu = R"(
----------------------------------------------------------------------------
[1]: 修改姓名, [2]: 修改年龄, [3]: 修改性别, [4] : 修改成绩, [0]: 退出修改
----------------------------------------------------------------------------
)";
            auto handle_error = [](bool state) {
                if (state) {
                    std::cout << "修改操作成功\n";
                } else {
                    std::cout << "修改失败, 请重试\n";
                }
            };

            while (true) {
                std::cout << menu << '\n' << "etid student)";

                std::string operation;
                std::getline(std::cin, operation);
                std::cout << operation << '\n';
                if (operation.find("1") != std::string::npos) {
                    // 修改姓名
                    std::cout << "修改姓名\n";
                    std::string edit_key {"name"};
                    client_server::Send(edit_key);

                    std::string target;
                    std::cout << "请输入修改后的姓名\nedit student)";
                    std::getline(std::cin, target);
                    client_server::Send(target);
                    handle_error(handle_state());
                } else if (operation.find("3") != std::string::npos) {
                    // 修改性别
                    std::cout << "修改性别\n";
                    std::string edit_key {"gender"};
                    client_server::Send(edit_key);

                    std::string target;
                    std::cout << "请输入修改后的性别\nedit student)";
                    std::getline(std::cin, target);
                    client_server::Send(target);
                    handle_error(handle_state());
                } else if (operation.find("0") != std::string::npos) {
                    // 退出修改
                    std::cout << "退出修改\n";
                    break;
                }
            }
        } else {
            std::cout << "查找失败\n";
        }
        // send(client_server::server_socket(), edit_number.c_str(), edit_number.size(), 0);
    }
}    // namespace client_manager