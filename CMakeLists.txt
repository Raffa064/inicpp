cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(IniCpp)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(inicpp STATIC ${SOURCES})
target_include_directories(inicpp PUBLIC "${PROJECT_SOURCE_DIR}/include")

set(INICPP_LIBRARIES
    "inicpp"
    PARENT_SCOPE)
