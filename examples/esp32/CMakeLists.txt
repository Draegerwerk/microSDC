cmake_minimum_required(VERSION 3.10)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MicroSDC)

target_add_binary_data(MicroSDC.elf "../../certs/ca.crt" TEXT)
target_add_binary_data(MicroSDC.elf "../../certs/server.crt" TEXT)
target_add_binary_data(MicroSDC.elf "../../certs/server.key" TEXT)
