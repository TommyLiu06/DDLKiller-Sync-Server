#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>

struct TodoItem {
    std::string uuid;
    std::string lastModified;
    std::string title;
    std::string description;
    std::string dueDate;
    int completeFlag;
};

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);

    // SQLite::Database 不可拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // void addModification(
    //     const std::string& operation,
    //     const int& targetId = 0,
    //     const std::string& title = "",
    //     const std::string& description = "",
    //     const std::string& dueDate = "",
    //     const int& deleteFlag = 0,
    //     const int& completeFlag = 0
    // );

    std::vector<TodoItem> getTodoItems();

private:
    SQLite::Database db;
    SQLite::Statement insert;
    SQLite::Statement select;

};
