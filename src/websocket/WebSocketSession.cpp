#include <iostream>
#include "WebSocketSession.h"
#include "WebSocketServer.h"
#include "../json/JsonParser.h"
#include "../json/JsonSender.h"

WebSocketSession::WebSocketSession(tcp::socket socket, WebSocketServer* server)
    : ws_(std::move(socket)), server_(server)
{}

void WebSocketSession::start()
{
    // WebSocket 握手
    ws_.accept();

    // 通知 server：此 session 加入连接列表
    server_->addClient(shared_from_this());

    // 开始读取消息
    do_read();
}

void WebSocketSession::do_read()
{
    auto buffer = new beast::flat_buffer;

    ws_.async_read(*buffer,
        [self = shared_from_this(), buffer](beast::error_code ec, std::size_t n)
        {
            self->on_read(ec, n, buffer);
        });
}

void WebSocketSession::on_read(beast::error_code ec, std::size_t, beast::flat_buffer* buffer)
{
    if (ec) {
        delete buffer;
        return on_close();
    }

    // 提取消息字符串
    std::string msg = boost::beast::buffers_to_string(buffer->data());
    delete buffer;

    // 解析 json
    std::string msgType = JsonParser::getMessageType(msg);

    // 处理不同类型的消息
    if (msgType == "modification") {
        std::string modType = JsonParser::getModificationType(msg);

        if (modType == "add") {
            AddOperation op = JsonParser::parseAddOperation(msg);

            if (op == AddOperation{}) {  // 与默认构造相同说明解析失败
                std::cerr << "Failed to parse add operation.\n";

            } else {
                TodoItem item;
                item.uuid = op.uuid;
                item.lastModified = op.uuid;  // 使用 uuid 作为创建时间戳
                item.title = op.title;
                item.description = op.description;
                item.dueDate = op.dueDate;
                item.completeFlag = 0;
                server_->broadcast(msg, shared_from_this(), modType, op.uuid);
                server_->dbManager.addTodoItem(item);
            }

        } else if (modType == "delete") {
            DeleteOperation op = JsonParser::parseDeleteOperation(msg);

            if (op == DeleteOperation{}) {  // 与默认构造相同说明解析失败
                std::cerr << "Failed to parse delete operation.\n";

            } else {
                server_->broadcast(msg, shared_from_this(), modType, op.targetUuid);
                server_->dbManager.deleteTodoItem(op.targetUuid);
            }

        } else if (modType == "modify") {
            ModifyOperation op = JsonParser::parseModifyOperation(msg);

            if (op == ModifyOperation{}) {  // 与默认构造相同说明解析失败
                std::cerr << "Failed to parse modify operation.\n";

            } else {
                TodoItem item;
                item.uuid = op.targetUuid;
                item.lastModified = op.lastModified;
                item.title = op.title;
                item.description = op.description;
                item.dueDate = op.dueDate;
                item.completeFlag = op.completeFlag;
                server_->dbManager.moidfyTodoItem(item);
                // 对于修改消息，直接广播
                server_->broadcast(msg, shared_from_this());
            }

        } else if (modType == "operation_error") {
            std::cerr << "Failed to parse the operation of incoming message." << std::endl;

        } else {
            std::cerr << "Unknown modification type: " << modType << std::endl;
            return;
        }

    } else if (msgType == "full_update_request") {
        // 获取当前服务器列表并发送给请求更新的客户端
        std::vector<TodoItem> items = server_->dbManager.getTodoItems();
        std::string fullUpdateMsg = JsonSender::createFullUpdateMessage(items);
        server_->callBack(fullUpdateMsg, shared_from_this());

    } else if (msgType == "full_update") {
        // 先用客户端发送的列表更新服务器数据库
        std::vector<TodoItem> items = JsonParser::parseTodoItems(msg);
        server_->dbManager.updateTodoItems(items);

        // 对除消息源以外的所有客户端广播修改后的的完整列表，不要求回复
        std::vector<TodoItem> updatedItems = server_->dbManager.getTodoItems();
        std::string fullUpdateMsg = JsonSender::createFullUpdateMessage(updatedItems, true);
        server_->broadcast(fullUpdateMsg, shared_from_this());

    } else if (msgType == "type_error") {
        std::cerr << "Failed to parse the type of incoming message." << std::endl;

    } else {
        // 其他类型的消息，暂不处理
        std::cerr << "Unknown message type: " << msgType << std::endl;
    }

    // 继续读下一条消息
    do_read();
}

void WebSocketSession::send(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(write_mutex_);
    ws_.text(true);
    ws_.write(boost::asio::buffer(msg));
}

void WebSocketSession::on_close()
{
    server_->removeClient(shared_from_this());
}
