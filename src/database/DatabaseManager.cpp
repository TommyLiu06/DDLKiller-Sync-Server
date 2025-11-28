#include "DatabaseManager.h"
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
    insert(db, "INSERT INTO modifications ("
        "uuid, "
        "last_modified, "
        "title, "
        "description, "
        "due_date, "
        "complete_flag"
        ") " 
        "VALUES (?, ?, ?, ?, ?, ?)"
    ),
    select(db, "SELECT * FROM modifications")
{
    try {
        db.exec(
            "CREATE TABLE IF NOT EXISTS modifications ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "uuid TEXT, "              // timestamp of creation  
            "last_modified TEXT, "     // timestamp of last modification
            "title TEXT, "             // operation is "add" or "modify"
            "description TEXT, "       // operation is "add" or "modify"
            "due_date TEXT, "          // operation is "add" or "modify"
            "complete_flag INTEGER DEFAULT 0"
            ");"
        );

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to open database: ") + e.what());
    }
}

std::vector<TodoItem> DatabaseManager::getTodoItems() {
    std::vector<TodoItem> todoItems;

    while (select.executeStep()) {
        TodoItem item;
        item.uuid = select.getColumn("uuid").getString();
        item.lastModified = select.getColumn("last_modified").getString();
        item.title = select.getColumn("title").getString();
        item.description = select.getColumn("description").getString();
        item.dueDate = select.getColumn("due_date").getString();
        item.completeFlag = select.getColumn("complete_flag").getInt();

        todoItems.push_back(item);
    }
    select.reset();

    return todoItems;
}
