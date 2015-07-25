cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")

option(DEBUG_INTERFACE "Add debug command parser" OFF)
if(${DEBUG_INTERFACE})
   add_definitions( -DDEBUG_INTRFC )
endif()

set( SRC_FILES
   blockbattle.cpp
   myai.cpp
   )

add_executable(blockbattle
   ${SRC_FILES}
   )


