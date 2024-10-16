# CMakeLists.txt (updated)
cmake_minimum_required(VERSION 3.5)
project(NekoIDE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# Build the NekoIDE executable
add_executable(NekoIDE main.cpp neko.c)

target_link_libraries(NekoIDE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${GTK3_LIBRARIES}
)

# Build the standalone NekoLang interpreter
add_executable(neko neko.c)

# Define BUILD_NEKO_INTERPRETER when building the interpreter
target_compile_definitions(neko PRIVATE BUILD_NEKO_INTERPRETER)

target_link_libraries(neko
        ${GTK3_LIBRARIES}
)
