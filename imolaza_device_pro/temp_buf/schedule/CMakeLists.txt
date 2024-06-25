

cmake_minimum_required(VERSION 2.8)



# SET(CMAKE_C_COMPILER "/usr/bin/clang")

# set (CMAKE_C_FLAGS "-m32")

#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++") 

# add_definitions("-Wall -g")

project(schedule)


# aux_source_directory(.  srcs  )

file(GLOB_RECURSE srcs .  *.c)

#INCLUDE_DIRS "."

MESSAGE( STATUS "${PROJECT_SOURCE_DIR}\n ${srcs} \n")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")

ADD_LIBRARY("TestBen" SHARED ${srcs} )

add_executable(${PROJECT_NAME} ${srcs})






