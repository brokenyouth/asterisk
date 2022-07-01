
cmake_minimum_required (VERSION 3.8)

add_executable (asterisk "asterisk.cpp" "asterisk.h")

set_property(TARGET asterisk PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:asterisk>")
target_include_directories(asterisk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(asterisk glm)
target_link_libraries(asterisk sdl2)