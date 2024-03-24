cmake_minimum_required(VERSION 3.5)

project(test_client_server)

set(SERVER_DIR Server)
set(LOGGER_DIR Logger)
set(CLIENT_DIR Client)
set(INCLUDE_DIR Server Client Logger)

set(SERVER_SOURCES main_server.cpp ${SERVER_DIR}/server.cpp ${LOGGER_DIR}/logger.cpp)
set(CLIENT_SOURCES main_client.cpp ${CLIENT_DIR}/client.cpp)

include_directories(${INCLUDE_DIR})
add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})
