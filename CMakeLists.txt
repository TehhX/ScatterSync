cmake_minimum_required(VERSION 3.28)
project(ScatterSync LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Extra steps may be required on different operating systems.
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("Building ScatterSync for Windows. Setting subsystem to Windows.")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Building ScatterSync for Linux. No extra steps required.")

else()
    message("Building ScatterSync for unsupported OS: " ${CMAKE_SYSTEM_NAME} ". Good luck.")

endif()

# wxWidgets is required for this software.
find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

# Source .cpp files.
set(SRC
    src/ScatterSyncApp.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "inc/")
