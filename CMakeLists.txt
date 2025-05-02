cmake_minimum_required(VERSION 3.28)
project(ScatterSync LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

set(SRC
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
include_directories(${PROJECT_NAME} PRIVATE "inc")
