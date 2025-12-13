#include "JsonParser.h"

std::string JsonParser::getModificationType(const std::string& jsonStr)
{
    try {
        auto jsonObj = nlohmann::json::parse(jsonStr);
        return jsonObj.at("content").at("operation").get<std::string>();
    }
    catch (...) {
        return "operation_error";
    }
}

AddOperation JsonParser::parseAddOperation(const std::string& jsonStr)
{
    try {
        auto jsonObj = nlohmann::json::parse(jsonStr);
        AddOperation op;
        auto& content = jsonObj.at("content");
        
        op.uuid = content.at("uuid").get<std::string>();
        op.title = content.at("title").get<std::string>();
        op.description = content.at("description").get<std::string>();
        op.dueDate = content.at("due_date").get<std::string>();
        return op;
    }
    catch (...) {
        return AddOperation{};  // 返回默认构造
    }
}

DeleteOperation JsonParser::parseDeleteOperation(const std::string& jsonStr)
{
    try {
        auto jsonObj = nlohmann::json::parse(jsonStr);
        DeleteOperation op;
        op.targetUuid = jsonObj.at("content").at("target_uuid").get<std::string>();
        return op;
    }
    catch (...) {
        return DeleteOperation{};  // 返回默认构造
    }
}

ModifyOperation JsonParser::parseModifyOperation(const std::string& jsonStr)
{
    try {
        auto jsonObj = nlohmann::json::parse(jsonStr);
        ModifyOperation op;
        auto& content = jsonObj.at("content");

        op.targetUuid = content.at("target_uuid").get<std::string>();
        op.lastModified = content.at("last_modified").get<std::string>();
        op.title = content.at("title").get<std::string>();
        op.description = content.at("description").get<std::string>();
        op.dueDate = content.at("due_date").get<std::string>();
        op.completeFlag = content.at("complete_flag").get<bool>();
        return op;
    }
    catch (...) {
        return ModifyOperation{};  // 返回默认构造
    }
}

std::string JsonParser::getMessageType(const std::string& jsonStr)
{
    try {
        auto jsonObj = nlohmann::json::parse(jsonStr);
        return jsonObj.at("type").get<std::string>();
    }
    catch (...) {
        return "type_error";
    }
}

std::vector<TodoItem> JsonParser::parseTodoItems(const std::string& jsonStr)
{
    try {
        std::vector<TodoItem> items;
        auto jsonObj = nlohmann::json::parse(jsonStr);
        
        // 使用 .at("content") 确保 content 存在
        for (const auto& itemJson : jsonObj.at("content")) {
            TodoItem item;
            item.uuid = itemJson.at("uuid").get<std::string>();
            item.lastModified = itemJson.at("last_modified").get<std::string>();
            item.title = itemJson.at("title").get<std::string>();
            item.description = itemJson.at("description").get<std::string>();
            item.dueDate = itemJson.at("due_date").get<std::string>();
            item.completeFlag = itemJson.at("complete_flag").get<bool>();
            items.push_back(item);
        }
        return items;
    }
    catch (...) {
        return {}; // 返回空 vector
    }
}
