cmake_minimum_required(VERSION 3.5)
project(NekoIDE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Build the NekoIDE executable
add_executable(NekoIDE main.cpp neko.c)

target_link_libraries(NekoIDE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

# Build the standalone NekoLang interpreter
add_executable(neko neko.c)

# Define BUILD_NEKO_INTERPRETER when building the interpreter
target_compile_definitions(neko PRIVATE BUILD_NEKO_INTERPRETER)
