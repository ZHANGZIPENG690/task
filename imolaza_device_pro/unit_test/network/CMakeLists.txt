

cmake_minimum_required(VERSION 2.8)



# SET(CMAKE_C_COMPILER "/usr/bin/clang")

# set (CMAKE_C_FLAGS "-m32")

#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++") 

# add_definitions("-Wall -g")

project(network)




function(GET_SUB_PATH IN_SRC OUT)
	string(FIND ${IN_SRC} "unit_test/"  index)
	string(LENGTH  ${IN_SRC} len)
	string(SUBSTRING  ${IN_SRC}  ${index} ${len}  out)
	string(REPLACE   "unit_test/"  "" result ${out})
	set(${OUT} ${result} PARENT_SCOPE )
endfunction()



GET_SUB_PATH( ${CMAKE_CURRENT_SOURCE_DIR}  FILE_PATH )



# MESSAGE( STATUS ${FILE_PATH})	 core/middle/headware/button


# aux_source_directory(.  srcs  )

#link_directories("/usr/libexec")

#target_link_libraries("python_plugin")


file(GLOB_RECURSE srcs  ../../${FILE_PATH}/main.c  ../../application/${PROJECT_NAME}/*.c )

#INCLUDE_DIRS "."

MESSAGE( STATUS "--> ${srcs} \n")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("../../")

ADD_LIBRARY("TestBen" SHARED ${srcs} )

add_executable(${PROJECT_NAME} ${srcs})






