cmake_minimum_required(VERSION 3.28)
project(ScatterSync LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# On Windows, setting the subsystem to Windows using the linker flags is required.
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

# wxWidgets is required for this software.
find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

# Source .cpp files.
set(SRC
    FileItem
    FileList
    GitControl
    MainFrame
    ManifestManip
    ScatterSyncApp
    SettingsFrame
    UserFileControl
)

# For each file "x" in SRC, transform to "src/x.cpp".
list(TRANSFORM SRC PREPEND "src/")
list(TRANSFORM SRC APPEND  ".cpp")

add_executable(ScatterSync ${SRC})

target_link_libraries(ScatterSync PRIVATE ${wxWidgets_LIBRARIES})
target_include_directories(ScatterSync PRIVATE "inc/")
