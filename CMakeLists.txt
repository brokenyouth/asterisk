cmake_minimum_required (VERSION 3.8)

project ("asterisk")

set (CMAKE_CXX_STANDARD 23)

add_subdirectory ("third_party")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory ("asterisk")
