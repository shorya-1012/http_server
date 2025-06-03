cmake_minimum_required(VERSION 4.0)

project(HTTP_Server)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(server 
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/server.cpp
  ${SOURCE_DIR}/parser.cpp
)

target_include_directories(server PRIVATE ${SOURCE_DIR})
