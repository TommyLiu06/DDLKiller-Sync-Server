#pragma once

#include <boost/asio.hpp>
#include <set>
#include <mutex>
#include <memory>
#include <string>
#include "WebSocketSession.h"
#include "../database/DatabaseManager.h"

class WebSocketServer
{
public:
    using tcp = boost::asio::ip::tcp;

    WebSocketServer(boost::asio::io_context& ioc, unsigned short port, const char* dbPath);
    DatabaseManager dbManager;

    // 启动监听
    void run();

    // Session 通知 Server：自己加入
    void addClient(std::shared_ptr<WebSocketSession> client);

    // Session 通知 Server：自己离开
    void removeClient(std::shared_ptr<WebSocketSession> client);

    // 广播给所有 session
    void broadcast(const std::string& msg, const std::shared_ptr<WebSocketSession>& exclude = nullptr);

    // 回应发送消息的 session
    void callBack(const std::string& msg, const std::shared_ptr<WebSocketSession>& source = nullptr);

private:
    tcp::acceptor acceptor_;
    std::set<std::shared_ptr<WebSocketSession>> clients_;
    std::mutex clients_mutex_;

    void do_accept();
};
