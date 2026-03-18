cmake_minimum_required(VERSION 3.10)
project(BasicFormulaCalc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main main.c)
target_link_libraries(main m)