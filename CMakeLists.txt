cmake_minimum_required (VERSION 2.8)
project (compiler)

add_definitions ("-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-dev ")
set(SOURCE_FILES main.c)
add_executable( compiler ${SOURCE_FILES})
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()

