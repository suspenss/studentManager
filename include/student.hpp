#pragma once
#include <sstream>
#include <string>

namespace manager {

    /// @brief this is a student struct, which storage the information for student.
    struct Student {
        /// @brief student name
        std::string name, number, gender;
        /// @brief student grade about math, chinese english ...
        unsigned int age, math, chinese, english;

        static std::string serializeStudent(const Student &student) {
            std::ostringstream oss;
            oss << student.name << " " << student.number << " " << student.gender << " " << student.age << " "
                << student.math << " " << student.chinese << " " << student.english;
            return oss.str();
        }

        // Deserialize a string to a Student object
        static Student deserializeStudent(const std::string &data) {
            Student student;
            std::istringstream iss(data);
            iss >> student.name >> student.number >> student.gender >> student.age >> student.math >> student.chinese >>
                student.english;
            return student;
        }
    };
};    // namespace manager
