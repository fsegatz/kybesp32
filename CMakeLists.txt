# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
add_compile_definitions("KYBER_90S")
add_compile_definitions("KYBER_K=2")
add_compile_definitions("SHA_ACC=1")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(kybesp32)