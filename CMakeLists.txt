cmake_minimum_required(VERSION 3.15)
project(PingPongEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 manually (for macOS/Homebrew)
find_path(SDL2_INCLUDE_DIR SDL.h
    HINTS /opt/homebrew/include/SDL2 /usr/local/include/SDL2
)
find_library(SDL2_LIBRARY SDL2
    HINTS /opt/homebrew/lib /usr/local/lib
)
include_directories(${SDL2_INCLUDE_DIR})

# Source files
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(PingPongEngine ${SOURCES})
target_link_libraries(PingPongEngine ${SDL2_LIBRARY})

add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PingPongEngine
    DEPENDS PingPongEngine
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# cmake_minimum_required(VERSION 3.15)
# project(PingPongEngine)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Find SDL3 manually (for macOS/Homebrew)
# # find_path(SDL3_INCLUDE_DIR SDL3
# #     HINTS /opt/homebrew/include/SDL3 /usr/local/include/SDL3
# # )
# # find_library(SDL3_LIBRARY SDL3
# #     HINTS /opt/homebrew/lib /usr/local/lib
# # )

# find_path(SDL_INCLUDE_DIR
#     NAMES SDL3/SDL.h SDL2/SDL.h SDL.h
#     HINTS /opt/homebrew/include /usr/local/include
# )
# find_library(SDL_LIBRARY
#     NAMES SDL3 SDL2 SDL
#     HINTS /opt/homebrew/lib /usr/local/lib
# )



# # include_directories(${SDL3_INCLUDE_DIR})

# # Source files
# file(GLOB SOURCES "src/*.cpp")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# # add_executable(PingPongEngine ${SOURCES})
# add_executable(PingPongEngine ${SOURCES})


# target_include_directories(PingPongEngine PRIVATE
#     ${SDL_INCLUDE_DIR}
#     ${SDL_INCLUDE_DIR}/SDL3
#     ${SDL_INCLUDE_DIR}/SDL2
# )

# target_link_libraries(PingPongEngine PRIVATE ${SDL_LIBRARY})

# add_custom_target(run
#     COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PingPongEngine
#     DEPENDS PingPongEngine
#     WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
# )
