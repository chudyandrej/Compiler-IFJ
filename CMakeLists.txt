cmake_minimum_required(VERSION 3.2)
project(interpret)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 ")
set(SOURCE_FILES main.c)
add_executable(interpret ${SOURCE_FILES} fake_skener.h fake_skener.c)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()