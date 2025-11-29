#include "WebSocketServer.h"
#include "WebSocketSession.h"
#include "../json/JsonSender.h"

WebSocketServer::WebSocketServer(boost::asio::io_context& ioc, unsigned short port, const char* dbPath)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)),
        dbManager(dbPath)
{}

void WebSocketServer::run()
{
    do_accept();
}

void WebSocketServer::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec) {
                auto session = std::make_shared<WebSocketSession>(std::move(socket), this);
                session->start();
            }
            do_accept();  // 接受下一个
        });
}

void WebSocketServer::addClient(std::shared_ptr<WebSocketSession> client)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);
    clients_.insert(client);
}

void WebSocketServer::removeClient(std::shared_ptr<WebSocketSession> client)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);
    clients_.erase(client);
}

void WebSocketServer::broadcast(const std::string& msg, const std::shared_ptr<WebSocketSession>& exclude)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    for (auto& client : clients_) {
        if (client) {
            if (client == exclude) {
                std::string successMsg = JsonSender::createSuccessMessage();
                try {
                    client->send(successMsg);  // 发送成功响应
                } catch (...) {
                    // 忽略客户端断开
                }

            } else {
                try {
                    client->send(msg);
                } catch (...) {
                    // 忽略某个客户端断开
                }
            }
            
        }
    }
}
