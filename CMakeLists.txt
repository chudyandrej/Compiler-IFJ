cmake_minimum_required(VERSION 3.3)
project(ifj)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic")

set(SOURCE_FILES main.c parser.c parser.h scanner.c scanner.h str.c str.h err.c err.h precedence.c precedence.h stack.c stack.h tac.c tac.h ial.c ial.h garbage.c garbage.h)
add_executable(ifj ${SOURCE_FILES})