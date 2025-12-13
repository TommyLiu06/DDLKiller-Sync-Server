#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>

struct TodoItem {
    std::string uuid = "";          // 唯一表标识符，用创建时间表示
    std::string lastModified = "";  // 最后修改的时间
    std::string title = "";         // 待办事项标题
    std::string description = "";   // 待办事项详细描述
    std::string dueDate = "";       // 待办事项逾期时间
    int completeFlag = -1;          // 待办事项完成标识 (1 - 已完成 / 0 - 未完成 / -1 - 未初始化)

    bool operator==(const AddOperation&) const = default;
};

class DatabaseManager
{
public:
    DatabaseManager(const char* dbPath);

    // SQLite::Database 不可拷贝
    DatabaseManager(const DatabaseManager&) = delete;

    // SQLite::Database 不可赋值
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    // 在服务器数据库添加 item
    void addTodoItem(const TodoItem& item);

    // 按 uuid 在服务器数据库找到对应项并删除
    void deleteTodoItem(const std::string& uuid);

    // 根据 item.uuid 在服务器数据库中查找对应事项并修改
    void moidfyTodoItem(const TodoItem& item);
    
    // 从客户端发送的列表全量更新服务器数据库
    void updateTodoItems(const std::vector<TodoItem>& clientTodoItems);

    // 从服务器数据库获取待办事项列表
    std::vector<TodoItem> getTodoItems();

private:
    SQLite::Database db;

    // 按照 uuid 从 source 中找到 exclude 中没有的项，返回 source 中 exclude 没有的项
    std::vector<TodoItem> getUnique(const std::vector<TodoItem>& source,
                                    const std::vector<TodoItem>& exclude);

    // 按照 uuid 找到 aItems 和 bItems 都有的项，返回 aItems 中 bItems 也有的项
    std::vector<TodoItem> getCommon(const std::vector<TodoItem>& aItems,
                                    const std::vector<TodoItem>& bItems);
    
};
