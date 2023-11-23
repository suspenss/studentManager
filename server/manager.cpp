#include <iostream>
#include <mutex>
#include <mysql/mysql.h>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <vector>
/// local
#include "../include/server.hpp"

///此行显示可异步工作
namespace manager {
    /// @brief this a simple enum that construct a Gender
    enum Gender {
        female,
        male,
    };

    /// @brief this is a student struct, which storage the information for student.
    struct Student {
        /// @brief student name
        std::string name, number;
        /// @brief student sexual
        Gender gender_;
        /// @brief student age and number
        /// @brief student grade about math, chinese english ...
        unsigned int age, math, chinese, english;

        /// @brief the function get the sexual as string literal
        /// @return the sexual string literal, man return "male", woman return "female", another return "unknow"
        const char *gender() const {
            if (gender_ == Gender::female) {
                return "female";
            } else if (gender_ == Gender::male) {
                return "male";
            } else {
                return "unknown";
            }
        }
    };

    /// @brief studentManager manage the student system, connect the mysql database and give the interface to client to
    /// manage student system
    class StudentManager {
      private:
        /// @brief Mysql database structure
        MYSQL database;
        /// @brief todo
        int socket_fd;
        /// Ensure that the operations performed are atomic operations
        mutable std::shared_mutex mutex_;

        StudentManager() {}

      public:
        static StudentManager &instance() {
            static StudentManager instance;
            return instance;
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
            for (int i = 0; i < num_fields; ++i) {
                info_result += fields[i].name;
                info_result += '\t';
            }
            info_result += '\n';

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)) != nullptr) {
                for (int i = 0; i < num_fields; ++i) {
                    info_result += row[i];
                    info_result += '\t';
                }
                info_result += '\n';
            }

            mysql_free_result(result);
            return {true, info_result};
        }

        bool add(Student &s) {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            std::string query(1024, '\0');
            sprintf(&query[0],
                "insert into users(name, studentnumber, age, chinese, math, english) values('%s', '%s', %d, '%s', %d, %d, %d);",
                s.name.data(), s.number.data(), s.age, s.gender(), s.chinese, s.math, s.english);

            auto [query_state, result] = mysql_query(query);
            std::cout << "Add student information: " << result << std::endl;

            return query_state;
        }

        bool remove(std::string_view delete_name) {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            std::string query(1024, '\0');
            sprintf(&query[0], "delete from users where studentnumber = '%s';", delete_name.data());

            auto [query_state, result] = mysql_query(query);
            std::cout << "Delete student information: " << result << std::endl;

            return query_state;
        }

        bool modify() {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            return false;
        }

        std::pair<bool, std::string> search_as(std::string_view key, std::string_view query_info) {
            std::shared_lock<std::shared_mutex> lock(mutex_);

            std::string query(120, '\0');
            sprintf(&query[0], "SELECT * FROM users where %s = %s;", key.data(), query_info.data());
            auto [query_state, query_result] = mysql_query(query);

            std::cout << "Search as" << key << " : " << query_result << std::endl;

            auto [storage_state, storage_result] = mysql_result();

            return {storage_state and query_state, storage_result};
        }

        std::pair<bool, std::string> show() {
            std::shared_lock<std::shared_mutex> lock(mutex_);
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
            msg = "Operation success!\n";
        } else {
            msg = "Operation failed!\n";
        }
        send(THREAD_SOCKET, msg.data(), msg.size(), 0);
    }

    void add() {
        std::string send_msg {"添加学生信息\n"};
        send(THREAD_SOCKET, send_msg.c_str(), send_msg.size(), 0);

        std::vector<std::string> send_msg_item {"学生姓名", "学生学号", "学生性别，[男/女]", "学生年龄",
            "学生成绩：数学成绩", "学生成绩：语文成绩", "学生成绩：英语成绩"};

        std::vector<std::string> student_info_vec;
        for (auto &item : send_msg_item) {
            std::string send_msg {"请输入信息：" + item};
            send(THREAD_SOCKET, send_msg.c_str(), send_msg.size(), 0);

            std::string recv_msg(200, '\0');
            auto recv_len = recv(THREAD_SOCKET, &recv_msg[0], recv_msg.size(), 0);
            std::cout << recv_msg << '\n';
            recv_msg.resize(recv_len);
            student_info_vec.emplace_back(recv_msg);
        }

        send_msg = "确认添加吗？[yes/no]\n";
        send(THREAD_SOCKET, send_msg.c_str(), send_msg.size(), 0);

        std::string confirm_info(10, '\0');
        recv(THREAD_SOCKET, &confirm_info[0], confirm_info.size(), 0);

        bool state = false;
        if (confirm_info.find("yes") or confirm_info.find("no") == -1) {
            Student s;
            s.name = student_info_vec[0];
            s.number = student_info_vec[1];
            s.gender_ = student_info_vec[2].find("男") == -1 ? Gender::female : Gender::male;
            s.age = std::stoi(student_info_vec[3]);
            s.math = std::stoi(student_info_vec[4]);
            s.english = std::stoi(student_info_vec[5]);
            s.chinese = std::stoi(student_info_vec[6]);
            state = StudentManager::instance().add(s);
        }

        handle_client_error(state);
    }

    void modify() {
        StudentManager::instance().modify();
    }

    void search() {
        std::string send_msg {"查询学生\n 请输入要查询的学生姓名\n"};
        send(THREAD_SOCKET, send_msg.c_str(), send_msg.size(), 0);
        std::string recv_msg(100, '\0');
        recv(THREAD_SOCKET, &recv_msg[0], recv_msg.size(), 0);
        std::string key {"name"};
        auto [state, result] = StudentManager::instance().search_as(key, recv_msg);
        handle_client_error(state);
        if (state) {
            send(THREAD_SOCKET, result.c_str(), result.size(), 0);
        }
    }

    void show() {
        auto [state, result] = StudentManager::instance().show();
        if (send(THREAD_SOCKET, result.data(), result.size(), 0) < 0) {
            state = false;
        }
        handle_client_error(state);
    }

    void remove() {
        constexpr std::string_view msg {"请输入要删除的学生的名字"};
        send(THREAD_SOCKET, msg.data(), msg.size(), 0);

        std::string delete_name(100, '\0');
        recv(THREAD_SOCKET, &delete_name[0], delete_name.size(), 0);
        bool state = StudentManager::instance().remove(delete_name);
        handle_client_error(state);
    }
}    // namespace manager