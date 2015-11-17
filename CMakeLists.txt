cmake_minimum_required(VERSION 3.3)
project(ijf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c parser.c parser.h scanner.c scanner.h str.c str.h err.c err.h precedence.c precedence.h stack.c stack.h tac.c tac.h bst.c bst.h)
add_executable(ijf ${SOURCE_FILES})