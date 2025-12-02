#include "JsonSender.h"

std::string JsonSender::createFullUpdateMessage(const std::vector<TodoItem>& items, const bool& force) {
    nlohmann::json jsonObj;
    jsonObj["type"] = force ? "force_full_update" : "full_update";
    jsonObj["content"] = nlohmann::json::array();

    for (const auto& item : items) {
        nlohmann::json itemJson;
        itemJson["uuid"] = item.uuid;
        itemJson["last_modified"] = item.lastModified;
        itemJson["title"] = item.title;
        itemJson["description"] = item.description;
        itemJson["due_date"] = item.dueDate;
        itemJson["complete_flag"] = item.completeFlag;
        jsonObj["content"].push_back(itemJson);
    }

    return jsonObj.dump();
}

std::string JsonSender::createSuccessMessage() {
    nlohmann::json jsonObj;
    jsonObj["type"] = "response";
    jsonObj["content"] = { {"status", "success"} };
    return jsonObj.dump();
}
