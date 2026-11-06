cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(nameProject "ail") 
project(${nameProject} CXX)

file(GLOB_RECURSE source CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${nameProject} ${source})

target_include_directories(${nameProject} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${nameProject} PRIVATE include)