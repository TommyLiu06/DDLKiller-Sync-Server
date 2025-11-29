#pragma once

#include "JsonParser.h"

class JsonSender {
public:
    static std::string createFullUpdateMessage(const std::vector<TodoItem>& items);
    static std::string createSuccessMessage();
};
