#include "JsonParser.h"

std::string JsonParser::getModificationType(const std::string& jsonStr) {
    auto jsonObj = nlohmann::json::parse(jsonStr);
    return jsonObj["content"]["operation"].get<std::string>();
}

AddOperation JsonParser::parseAddOperation(const std::string& jsonStr) {
    auto jsonObj = nlohmann::json::parse(jsonStr);
    AddOperation op;
    op.uuid = jsonObj["content"]["uuid"].get<std::string>();
    op.title = jsonObj["content"]["title"].get<std::string>();
    op.description = jsonObj["content"]["description"].get<std::string>();
    op.dueDate = jsonObj["content"]["due_date"].get<std::string>();
    return op;
}

DeleteOperation JsonParser::parseDeleteOperation(const std::string& jsonStr) {
    auto jsonObj = nlohmann::json::parse(jsonStr);
    DeleteOperation op;
    op.targetUuid = jsonObj["content"]["target_uuid"].get<std::string>();
    return op;
}

ModifyOperation JsonParser::parseModifyOperation(const std::string& jsonStr) {
    auto jsonObj = nlohmann::json::parse(jsonStr);
    ModifyOperation op;
    op.targetUuid = jsonObj["content"]["target_uuid"].get<std::string>();
    op.lastModified = jsonObj["content"]["last_modified"].get<std::string>();
    op.title = jsonObj["content"]["title"].get<std::string>();
    op.description = jsonObj["content"]["description"].get<std::string>();
    op.dueDate = jsonObj["content"]["due_date"].get<std::string>();
    op.completeFlag = jsonObj["content"]["complete_flag"].get<int>();
    return op;
}

std::string JsonParser::getMessageType(const std::string& jsonStr) {
    try {
        auto jsonObj = nlohmann::json::parse(jsonStr);
        return jsonObj["type"].get<std::string>();
    }
    catch (...) {
        return "parse_error";
    }
}

std::vector<TodoItem> JsonParser::parseTodoItems(const std::string& jsonStr) {
    std::vector<TodoItem> items;
    auto jsonObj = nlohmann::json::parse(jsonStr);
    for (const auto& itemJson : jsonObj["content"]) {
        TodoItem item;
        item.uuid = itemJson["uuid"].get<std::string>();
        item.lastModified = itemJson["last_modified"].get<std::string>();
        item.title = itemJson["title"].get<std::string>();
        item.description = itemJson["description"].get<std::string>();
        item.dueDate = itemJson["due_date"].get<std::string>();
        item.completeFlag = itemJson["complete_flag"].get<int>();
        items.push_back(item);
    }
    return items;
}
