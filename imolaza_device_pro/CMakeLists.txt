cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(hot_so_fast)












# target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/ca.crt" TEXT)
# target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/client.crt" TEXT)
# target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/client.key" TEXT)


target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/cert/ca.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/cert/client.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/cert/client.key" TEXT)
