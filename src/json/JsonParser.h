#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include "../database/DatabaseManager.h"

struct AddOperation {
    std::string uuid = "";
    std::string title = "";
    std::string description = "";
    std::string dueDate = "";

    bool operator==(const AddOperation&) const = default;
};

struct DeleteOperation {
    std::string targetUuid = "";

    bool operator==(const AddOperation&) const = default;
};

struct ModifyOperation {
    std::string targetUuid = "";
    std::string lastModified = "";
    std::string title = "";
    std::string description = "";
    std::string dueDate ="";
    int completeFlag = -1;

    bool operator==(const AddOperation&) const = default;
};

class JsonParser
{
public:
    // 获取消息类型
    static std::string getMessageType(const std::string& jsonStr);

    // 获取修改操作类型
    static std::string getModificationType(const std::string& jsonStr);

    // 解析添加操作
    static AddOperation parseAddOperation(const std::string& jsonStr);

    // 解析删除操作
    static DeleteOperation parseDeleteOperation(const std::string& jsonStr);

    // 解析修改操作
    static ModifyOperation parseModifyOperation(const std::string& jsonStr);

    // 解析来自客户端的全量更新列表
    static std::vector<TodoItem> parseTodoItems(const std::string& jsonStr);
};
