
cmake_minimum_required (VERSION 3.8)

add_executable (asterisk "Main.cpp" "Game.h" "Game.cpp" "Logger.h" "Logger.cpp" "Renderer.h" "Renderer.cpp" "InputManager.h" "InputManager.cpp" "PlayerManager.h" "PlayerManager.cpp" "Player.h" "Player.cpp")

set_property(TARGET asterisk PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:asterisk>")
target_include_directories(asterisk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

if(NOT TARGET glm)
    # Stand-alone build
    find_package(glm REQUIRED)
endif()


if(NOT TARGET sdl2)
    # Stand-alone build
    find_package(sdl2 REQUIRED)
endif()

target_link_libraries(asterisk spdlog)
target_link_libraries(asterisk glm)
target_link_libraries(asterisk sdl2)