#pragma once

namespace manager {
    // 初始化学生管理器
    void init_manager();

    // 添加学生信息
    bool add();

    // 修改学生信息
    bool modify();

    // 删除学生信息
    bool remove();

    // 搜索学生信息
    bool search();
}    // namespace manager