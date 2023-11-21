#include <iostream>
#include <mysql/mysql.h>
#include <string>

enum Sexual {
    female,
    male,
};

struct Student {
    std::string name;
    Sexual _sexual;
    unsigned int age, number;
    unsigned int math, chinese, english;

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

class StudentManager {
  private:
    MYSQL database;
    int socket_fd;

    StudentManager() {}

  public:
    static StudentManager &getInstance() {
        static StudentManager instance;
        return instance;
    }

    void init() {
        if (mysql_init(&database) == nullptr) {
            std::cout << "init is error" << mysql_error(&database) << std::endl;
            exit(EXIT_FAILURE);
        }

        if (mysql_real_connect(&database, "localhost", "root", "1202", "studentInfo", 3306, nullptr, 0) == nullptr) {
            std::cout << "mysql_real_connect(): " << mysql_error(&database) << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Connected MySQL successful!" << std::endl;

        std::string query {
            "create table if not exists users(name varchar(100), studentnumber varchar(100), age int(50), gender varchar(50), chinese int(20), math int(20), english int(20));"};
        auto [state, result] = mysql_query(query);
        std::cout << "create tables : " << result << std::endl;
        if (state == false) {
            exit(EXIT_FAILURE);
        }
    }

    std::pair<bool, std::string> mysql_query(std::string query_info) {
        if (mysql_real_query(&database, query_info.c_str(), query_info.size()) == 0) {
            return {true, std::string("success !")};
        } else {
            return {false, std::string(mysql_error(&database))};
        }
    }

    bool add(Student s) {
        std::string query(1024, '\0');
        sprintf(query.data(),
            "insert into users(name, studentnumber, age, 语文, 数学, 英语) values('%s', %d, %d, '%s', %d, %d, %d);",
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
