cmake_minimum_required(VERSION 3.16)
project(VoleMachine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(vole
    gui.cpp
    Muhammad.cpp
    Eslam.cpp
    Nabil.cpp
    Abdelrahman.cpp
)

target_link_libraries(vole PRIVATE Qt6::Widgets)