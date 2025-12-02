#pragma once

#include "JsonParser.h"

class JsonSender {
public:
    // 创建 std::string 类型的全量更新消息，可以指定是否强制
    static std::string createFullUpdateMessage(const std::vector<TodoItem>& items, const bool& force = false);

    static std::string createSuccessMessage();
};
