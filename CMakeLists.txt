cmake_minimum_required(VERSION 3.28)
project(ScatterSync LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC})
include_directories(${PROJECT_NAME} PRIVATE "inc")
