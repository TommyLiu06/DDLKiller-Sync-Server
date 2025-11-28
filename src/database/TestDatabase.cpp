#include "DatabaseManager.h"
#include <iostream>

int main() {
    DatabaseManager dbManager("test.db");

    // Create a TodoItem
    TodoItem item1;
    item1.uuid = "20251129030444123";
    item1.lastModified = item1.uuid;
    item1.title = "Test Todo";
    item1.description = "This is a test todo item.";
    item1.dueDate = "2025-12-31";
    item1.completeFlag = 0;

    // Create another TodoItem
    TodoItem item2;
    item2.uuid = "20251129030444124";
    item2.lastModified = item2.uuid;
    item2.title = "Another Todo";
    item2.description = "This is another test todo item.";
    item2.dueDate = "2025-12-31";
    item2.completeFlag = 0;

    // Add TodoItems to the database
    dbManager.addTodoItem(item1);
    dbManager.addTodoItem(item2);

    // Retrieve and print all TodoItems
    std::vector<TodoItem> todoItems = dbManager.getTodoItems();
    for (const auto& item : todoItems) {
        std::cout << "-------------------------\n"
                    << "UUID: " << item.uuid << "\n"
                    << "Last Modified: " << item.lastModified << "\n"
                    << "Title: " << item.title << "\n"
                    << "Description: " << item.description << "\n"
                    << "Due Date: " << item.dueDate << "\n"
                    << "Complete Flag: " << item.completeFlag << "\n"
                    << "------------------------\n";
    }

    // Delete a TodoItem
    dbManager.deleteTodoItem(item1.uuid);

    // Verify deletion
    todoItems = dbManager.getTodoItems();
    std::cout << "After deletion:\n";
    for (const auto& item : todoItems) {
        std::cout << "-------------------------\n"
                    << "UUID: " << item.uuid << "\n"
                    << "Last Modified: " << item.lastModified << "\n"
                    << "Title: " << item.title << "\n"
                    << "Description: " << item.description << "\n"
                    << "Due Date: " << item.dueDate << "\n"
                    << "Complete Flag: " << item.completeFlag << "\n"
                    << "------------------------\n";
    }

}
