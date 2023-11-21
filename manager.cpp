#include <iostream>
#include <mysql/mysql.h>
#include <string>

/// @brief this a simple enum that construct a Sexual
enum Sexual {
    female,
    male,
};

/// @brief this is a student struct, which storage the information for student.
struct Student {
    /// @brief student name
    std::string name;
    /// @brief student sexual
    Sexual _sexual;
    /// @brief student age and number
    unsigned int age, number;
    /// @brief student grade about math, chinese english ...
    unsigned int math, chinese, english;

    /// @brief the function get the sexual as string literal
    /// @return the sexual string literal, man return "male", woman return "female", another return "unknow"
    const char *sexual() {
        if (_sexual == Sexual::female) {
            return "female";
        } else if (_sexual == Sexual::male) {
            return "male";
        } else {
            return "unknow";
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

    StudentManager() {}

  public:
    static StudentManager &getInstance() {
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
        if (mysql_real_connect(&database, "localhost", "root", "1202", "studentInfo", 3306, nullptr, 0) == nullptr) {
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
            /// mysql_error() return a string to describ error information
            return {false, std::string(mysql_error(&database))};
        }
    }

    bool add(Student s) {
        std::string query(1024, '\0');
        sprintf(query.data(),
            "insert into users(name, studentnumber, age, chinese, math, english) values('%s', %d, %d, '%s', %d, %d, %d);",
            s.name, s.number, s.age, s.sexual(), s.chinese, s.math, s.english);

        auto [query_state, result] = mysql_query(query);
        std::cout << "Add student information: " << result << std::endl;

        return query_state;
    }

    bool remove() {
        // todo
        return false;
    }

    bool modify() {
        return false;
    }

    bool search() {
        return false;
    }
};

static StudentManager &STUDENT_MANAGER = StudentManager::getInstance();

void init_manager() {
    STUDENT_MANAGER.init();
}

bool add() {
    Student s;

    return STUDENT_MANAGER.add(s);
}

bool modify() {
    return STUDENT_MANAGER.modify();
}

bool remove() {
    return STUDENT_MANAGER.remove();
}

bool search() {
    return STUDENT_MANAGER.search();
}
