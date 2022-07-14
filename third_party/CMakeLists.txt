add_subdirectory(spdlog INTERFACE)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(sdl2 INTERFACE)
set(sdl2_DIR "SDL_PATH" CACHE PATH "Path to SDL2")

add_library(sdl2_image INTERFACE)
set(sdl2_image_DIR "SDL2_image_PATH" CACHE PATH "Path to SDL2_image")

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb)


if (WIN32)
target_include_directories(sdl2 INTERFACE ${sdl2_DIR}/include)
target_link_directories(sdl2 INTERFACE ${sdl2_DIR}/lib/x64)
elseif (UNIX)
target_include_directories(sdl2 INTERFACE ${sdl2_DIR}/include/SDL2)
target_link_directories(sdl2 INTERFACE ${sdl2_DIR}/lib)
endif()

if (WIN32)
target_include_directories(sdl2_image INTERFACE ${sdl2_image_DIR}/include)
target_link_directories(sdl2_image INTERFACE ${sdl2_image_DIR}/lib/x64)
elseif (UNIX)
target_include_directories(sdl2_image INTERFACE ${sdl2_image_DIR}/include/SDL2)
target_link_directories(sdl2_image INTERFACE ${sdl2_image_DIR}/lib)
endif()

target_link_libraries(sdl2 INTERFACE SDL2 SDL2main)
target_link_libraries(sdl2_image INTERFACE SDL2 SDL2_image)

target_link_libraries(sdl2)
target_link_libraries(sdl2_image)
target_link_libraries(stb_image)