
cmake_minimum_required (VERSION 3.8)

add_executable (asterisk "Main.cpp" "Utility.h" "Game.h" "Game.cpp" "Logger.h" "Logger.cpp" "Color.h" "Renderer.h" "Renderer.cpp" "InputManager.h" "InputManager.cpp" "Actor.h" "Actor.cpp" "PlayerManager.h" "PlayerManager.cpp" "Player.h" "Player.cpp" "Command.h" "Command.cpp" "Level.h" "Level.cpp" "Texture.h" "Texture.cpp" "ResourceManager.h" "ResourceManager.cpp" "State.h" "State.cpp" "GameplayState.h" "GameplayState.cpp")

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

if(NOT TARGET sdl2_image)
    # Stand-alone build
    find_package(sdl2_image REQUIRED)
endif()

target_link_libraries(asterisk spdlog)
target_link_libraries(asterisk glm)
target_link_libraries(asterisk sdl2)
target_link_libraries(asterisk sdl2_image)
target_link_libraries(asterisk stb_image)

