cmake_minimum_required(VERSION 2.8)
project(interpret)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ")
set(SOURCE_FILES main.c)
add_executable(interpret ${SOURCE_FILES} syntax_checker.h syntax_checker.c stack.c stack.h scanner.c scanner.h str.h str.c)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()
