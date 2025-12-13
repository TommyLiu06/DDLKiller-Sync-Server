cmake_minimum_required(VERSION 3.10)
project(ddl_sync_server)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(SQLiteCpp REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(ddl_sync_server
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/json/JsonParser.cpp
    src/json/JsonSender.cpp
    src/websocket/WebSocketServer.cpp
    src/websocket/WebSocketSession.cpp
)

target_include_directories(ddl_sync_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ddl_sync_server PRIVATE Boost::system Boost::thread pthread)

target_include_directories(ddl_sync_server PRIVATE ${SQLITECPP_INCLUDE_DIRS})
target_link_libraries(ddl_sync_server PRIVATE SQLiteCpp sqlite3)

target_include_directories(ddl_sync_server PRIVATE ${NLOHMANN_JSON_INCLUDE_DIRS})
target_link_libraries(ddl_sync_server PRIVATE nlohmann_json::nlohmann_json)
