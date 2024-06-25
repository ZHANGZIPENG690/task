

cmake_minimum_required(VERSION 2.8)



# SET(CMAKE_C_COMPILER "/usr/bin/clang")

# set (CMAKE_C_FLAGS "-m32")

#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++") 

# add_definitions("-Wall -g")

project(user_main)





#set(CMAKE_BUILD_TYPE Debug) #debug模式下可以直接定位到代码coredown的位置
#add_subdirectory(./backward-cpp)

function(GET_SUB_PATH IN_SRC OUTA OUTB)
	string(FIND ${IN_SRC} "unit_test/"  index)
	string(LENGTH  ${IN_SRC} len)
	string(SUBSTRING  ${IN_SRC}  0 ${index}   outa)
	string(SUBSTRING  ${IN_SRC}  ${index} ${len}  outb)
	string(REPLACE   "unit_test/"  "" result ${outb})
	set(${OUTA} ${outa} PARENT_SCOPE )
	set(${OUTB} ${result} PARENT_SCOPE )
endfunction()



GET_SUB_PATH( ${CMAKE_CURRENT_SOURCE_DIR}  GLOBAL_FILE_PATH WELL_FILE_PATH   )



MESSAGE( STATUS "A " ${GLOBAL_FILE_PATH})	# 	 /home/ornai/modules/unit/molaza_device_pro/imolaza_device_pro/
MESSAGE( STATUS "A " ${WELL_FILE_PATH})	    #    core/middle/headware/button	
MESSAGE( STATUS "A " ${PROJECT_SOURCE_DIR})	#    /home/ornai/modules/unit/molaza_device_pro/imolaza_device_pro/unit_test/core/middle/headware/button
# MESSAGE( STATUS ${GLOBAL_FILE_PATH}${WELL_FILE_PATH})	#  /home/ornai/modules/unit/molaza_device_pro/imolaza_device_pro/unit_test/core/middle/headware/button


# aux_source_directory(.  srcs  )

#link_directories("/usr/libexec")

#target_link_libraries("python_plugin")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/")

file(GLOB_RECURSE srcs 
  	          ${PROJECT_SOURCE_DIR}/main.c    
		  #${GLOBAL_FILE_PATH}*.c
	        #   ${GLOBAL_FILE_PATH}unit_test/application/user_main/init.c 	  
	        #   ${GLOBAL_FILE_PATH}unit_test/application/user_main/ext_fun.c
	          ${GLOBAL_FILE_PATH}core/*.c	  
	          ${GLOBAL_FILE_PATH}main/*.c	  
	          ${GLOBAL_FILE_PATH}drive/*.c	  

		#   ${GLOBAL_FILE_PATH}unit_test/application/event_dicrt/*.c
		#   ${GLOBAL_FILE_PATH}unit_test/network/*.c
		#   ${GLOBAL_FILE_PATH}unit_test/drive/*.c
		#   ${GLOBAL_FILE_PATH}unit_test/application/network/*.c
		 )

#INCLUDE_DIRS "."

MESSAGE( STATUS "${srcs}")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${GLOBAL_FILE_PATH})

ADD_LIBRARY(${PROJECT_NAME}  SHARED ${srcs} )

#add_backward(${PROJECT_NAME})

#target_link_libraries(${PROJECT_NAME} pthread dl ${OPENCV_LIB})
# link_libraries( paho-mqtt3a)
link_libraries( m)


add_executable("${PROJECT_NAME}.a" ${srcs})






