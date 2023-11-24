#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <mutex>
#include <mysql/mysql.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <vector>
/// local
#include "../include/server_manager.hpp"
#include "../include/server_server.hpp"
#include "../include/student.hpp"

namespace server_manager {
    /// @brief studentManager manage the student system, connect the mysql database and give the interface to client to
    /// manage student system
    class StudentManager {
      private:
        /// @brief Mysql database structure
        MYSQL database;
        /// @brief todo
        int socket_fd;
        /// Ensure that the operations performed are atomic operations
        mutable std::mutex mutex_;

        // 构造函数
        StudentManager() {}

      public:
        // 单例模式
        static StudentManager &instance() {
            static StudentManager instance;
            return instance;
        }
        ~StudentManager() {
            mysql_close(&database);
        }
        /// @brief init the struct
        void init() {
            /// link the database and system mysql service
            if (mysql_init(&database) == nullptr) {
                std::cout << "init is error" << mysql_error(&database) << std::endl;
                exit(EXIT_FAILURE);
            }

            /// conncet a specific database to the manager
            /// hose is "localhost", user is "root", passwd is "1202", database name i "studentInfo"
            if (mysql_real_connect(&database, "localhost", "root", "1202", "studentInfo", 3306, nullptr, 0) ==
                nullptr) {
                std::cout << "mysql_real_connect(): " << mysql_error(&database) << std::endl;
                exit(EXIT_FAILURE);
            }

            std::cout << "Connected MySQL successful!" << std::endl;

            /// create the table in the studentInfo database
            std::string query {
                "create table if not exists users(name varchar(100), studentnumber varchar(100), age int(50), gender varchar(50), chinese int(20), math int(20), english int(20));"};

            auto [state, result] = mysql_query(query);
            std::cout << "create tables : " << result << std::endl;

            if (state == false) {
                exit(EXIT_FAILURE);
            }

            /// if cant init complete, program will exit.
        }

        /// @brief 封装了的 mysql 询问
        /// @param query_info 指定的 mysql 语句
        /// @return 第一个为一个布尔值，表示成功与否，第二个则表示返回的字符串提示信息
        std::pair<bool, std::string> mysql_query(std::string query_info) {
            if (mysql_real_query(&database, query_info.c_str(), query_info.size()) == 0) {
                return {true, std::string("success !")};
            } else {
                return {false, std::string(mysql_error(&database))};
            }
        }

        std::pair<bool, std::string> mysql_result() {
            std::string info_result;
            MYSQL_RES *result = mysql_store_result(&database);

            if (result == nullptr) {
                std::cerr << "mysql_store_result(): " << mysql_error(&database) << std::endl;
                return {false, "error"};
            }

            int num_fields = mysql_num_fields(result);
            MYSQL_FIELD *fields = mysql_fetch_fields(result);
            // Calculate maximum width for each column
            // Header
            for (int i = 0; i < num_fields; ++i) {
                info_result += fields[i].name;
                info_result += "\t\t";
            }
            info_result += '\n';

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)) != nullptr) {
                for (int i = 0; i < num_fields; ++i) {
                    info_result += row[i];
                    info_result += "\t\t";
                }
                info_result += '\n';
            }

            mysql_free_result(result);
            return {true, info_result};
        }

        bool add(manager::Student &s) {
            std::lock_guard<std::mutex> lock(mutex_);
            std::string query(1024, '\0');
            sprintf(&query[0],
                "insert into users(name, studentnumber, gender, age, chinese, math, english) values('%s', '%s', '%s', %d, %d, %d, %d);",
                s.name.c_str(), s.number.c_str(), s.gender.c_str(), s.age, s.chinese, s.math, s.english);

            auto [query_state, result] = mysql_query(query);
            std::cout << "Add student information: " << result << std::endl;

            return query_state;
        }

        bool remove(std::string_view delete_name) {
            std::lock_guard<std::mutex> lock(mutex_);
            std::string query(1024, '\0');
            sprintf(&query[0], "delete from users where name = '%s';", delete_name.data());

            auto [query_state, result] = mysql_query(query);
            std::cout << "Delete student information: " << result << std::endl;

            return query_state;
        }

        bool modify() {
            std::lock_guard<std::mutex> lock(mutex_);
            return false;
        }

        std::pair<bool, std::string> search_as(std::string_view key, std::string_view query_info) {
            std::lock_guard<std::mutex> lock(mutex_);

            std::string query(120, '\0');
            sprintf(&query[0], "SELECT * FROM users where %s = '%s';", key.data(), query_info.data());
            auto [query_state, query_result] = mysql_query(query);

            std::cout << "Search as" << key << " : " << query_result << std::endl;

            auto [storage_state, storage_result] = mysql_result();
            std::cout << storage_result << std::endl;
            return {storage_state and query_state, storage_result};
        }

        std::pair<bool, std::string> show() {
            std::lock_guard<std::mutex> lock(mutex_);
            mysql_query("SELECT * FROM users");
            return mysql_result();
        }
    };

    // 修改为 c++ 11 的单例模式 magic static，并且 lazy init

    void init_manager() {
        StudentManager::instance().init();
    }

    void handle_client_error(bool state) {
        std::string_view msg;
        if (state) {
            msg = "true";
        } else {
            msg = "true";
        }
        server::Send(msg);
        // send(THREAD_SOCKET, msg.data(), msg.size(), 0);
    }

    void add() {
        std::cout << "add function" << std::endl;
        std::string recv_info;
        server::Recv(recv_info);
        // recv(THREAD_SOCKET, &recv_info[0], recv_info.size(), 0);
        std::cout << "RECV: (add func)" << recv_info << '\n';

        if (recv_info.find("true") != std::string::npos) {
            std::string recv_student;
            server::Recv(recv_student);
            // recv(THREAD_SOCKET, &recv_student[0], recv_student.size(), 0);
            std::cout << "RECV: " << recv_student << '\n';
            // recv_student.resize(recv_len);
            manager::Student s = manager::Student::deserializeStudent(recv_student);
            bool state = StudentManager::instance().add(s);
            handle_client_error(state);
            std::cout << "here!";
        }
    }

    void modify() {
        std::string send_msg {"修改学生信息\n请输入要修改的学生学号\n"};
        server::Send(send_msg);
        // send(THREAD_SOCKET, send_msg.c_str(), send_msg.size(), 0);
        std::string recv_number;
        server::Recv(recv_number);
        // recv(THREAD_SOCKET, &recv_number[0], recv_number.size(), 0);
        std::string key {"number"};
        auto [state, result] = StudentManager::instance().search_as(key, recv_number);

        handle_client_error(state);
        // TODO
        // if (state) {
        //     send(THREAD_SOCKET, result.c_str(), result.size(), 0);
        // } else {
        // }

        StudentManager::instance().modify();
    }

    void search() {
        std::string recv_msg;
        server::Recv(recv_msg);
        // recv(THREAD_SOCKET, &recv_msg[0], recv_msg.size(), 0);
        std::string key {"name"};
        std::cout << recv_msg;
        auto [state, result] = StudentManager::instance().search_as(key, recv_msg);
        handle_client_error(state);
        std::cout << state << ' ' << result << ' ' << THREAD_SOCKET << std::endl;
        if (state) {
            server::Send(result);
            // send(THREAD_SOCKET, result.c_str(), result.size(), 0);
        }
    }

    void show() {
        auto [state, result] = StudentManager::instance().show();
        handle_client_error(state);

        server::Send(result);
        // send(THREAD_SOCKET, result.c_str(), result.size(), 0);

        std::cout << result;
        std::cout << std::endl << result.size() << '\n';
    }

    void remove() {
        std::string delete_name;
        server::Recv(delete_name);
        // recv(THREAD_SOCKET, &delete_name[0], delete_name.size(), 0);

        bool state = StudentManager::instance().remove(delete_name);
        handle_client_error(state);
    }
}    // namespace server_manager
