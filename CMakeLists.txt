cmake_minimum_required(VERSION 2.8)
project(interpret)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(SOURCE_FILES main.c)

add_executable(interpret ${SOURCE_FILES} syntax_checker.h syntax_checker.c stack.c stack.h scanner.c scanner.h str.h str.c err.c err.h)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "-O2")        ## Optimize
    set(CMAKE_C_FLAGS "-Wall")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()
