set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "Starting CMake configuration...")
cmake_minimum_required(VERSION 3.15)
project(PingPongEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to enable automatic SDL2 installation
option(AUTO_INSTALL_SDL2 "Automatically install SDL2 if not found" ON)

# Function to install SDL2 on different platforms
function(install_sdl2)
    message(STATUS "SDL2 not found. Attempting automatic installation...")
    
    if(WIN32)
        message(STATUS "Downloading SDL2 for Windows...")
        set(SDL2_VERSION "2.28.5")
        set(SDL2_URL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-VC.zip")
        set(SDL2_ZIP "${CMAKE_BINARY_DIR}/SDL2.zip")
        set(SDL2_EXTRACT_DIR "${CMAKE_SOURCE_DIR}/SDL2")
        
        file(DOWNLOAD ${SDL2_URL} ${SDL2_ZIP}
            SHOW_PROGRESS
            STATUS DOWNLOAD_STATUS
        )
        
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download SDL2")
        endif()
        
        file(ARCHIVE_EXTRACT INPUT ${SDL2_ZIP} DESTINATION ${CMAKE_BINARY_DIR}/SDL2_temp)
        file(GLOB SDL2_EXTRACTED "${CMAKE_BINARY_DIR}/SDL2_temp/SDL2-*")
        file(RENAME ${SDL2_EXTRACTED} ${SDL2_EXTRACT_DIR})
        file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/SDL2_temp")
        file(REMOVE ${SDL2_ZIP})
        
        set(SDL2_DIR ${SDL2_EXTRACT_DIR} PARENT_SCOPE)
        message(STATUS "SDL2 installed to ${SDL2_EXTRACT_DIR}")
        
    elseif(APPLE)
        message(STATUS "Installing SDL2 via Homebrew...")
        find_program(BREW brew)
        
        if(NOT BREW)
            message(FATAL_ERROR "Homebrew not found. Please install Homebrew from https://brew.sh")
        endif()
        
        execute_process(
            COMMAND ${BREW} install sdl2
            RESULT_VARIABLE BREW_RESULT
            OUTPUT_VARIABLE BREW_OUTPUT
            ERROR_VARIABLE BREW_ERROR
        )
        
        if(NOT BREW_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to install SDL2 via Homebrew: ${BREW_ERROR}")
        endif()
        
        message(STATUS "SDL2 installed successfully via Homebrew")
        
    else()
        # Linux
        message(STATUS "Attempting to install SDL2 on Linux...")
        
        # Detect package manager
        find_program(APT apt-get)
        find_program(DNF dnf)
        find_program(PACMAN pacman)
        find_program(ZYPPER zypper)
        
        if(APT)
            message(STATUS "Using apt-get to install SDL2...")
            execute_process(
                COMMAND sudo apt-get update
                COMMAND sudo apt-get install -y libsdl2-dev
                RESULT_VARIABLE INSTALL_RESULT
            )
        elseif(DNF)
            message(STATUS "Using dnf to install SDL2...")
            execute_process(
                COMMAND sudo dnf install -y SDL2-devel
                RESULT_VARIABLE INSTALL_RESULT
            )
        elseif(PACMAN)
            message(STATUS "Using pacman to install SDL2...")
            execute_process(
                COMMAND sudo pacman -S --noconfirm sdl2
                RESULT_VARIABLE INSTALL_RESULT
            )
        elseif(ZYPPER)
            message(STATUS "Using zypper to install SDL2...")
            execute_process(
                COMMAND sudo zypper install -y libSDL2-devel
                RESULT_VARIABLE INSTALL_RESULT
            )
        else()
            message(FATAL_ERROR "No supported package manager found. Please install SDL2 manually.")
        endif()
        
        if(NOT INSTALL_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to install SDL2")
        endif()
        
        message(STATUS "SDL2 installed successfully")
    endif()
endfunction()

# Platform-specific SDL2 configuration
if(WIN32)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2" CACHE PATH "Path to SDL2 directory")
    
    find_path(SDL2_INCLUDE_DIR SDL.h
        HINTS 
        ${SDL2_DIR}/include
        $ENV{SDL2_DIR}/include
    )
    
    find_library(SDL2_LIBRARY 
        NAMES SDL2
        HINTS 
        ${SDL2_DIR}/lib/x64
        ${SDL2_DIR}/lib
        $ENV{SDL2_DIR}/lib/x64
        $ENV{SDL2_DIR}/lib
    )
    
    find_library(SDL2MAIN_LIBRARY 
        NAMES SDL2main
        HINTS 
        ${SDL2_DIR}/lib/x64
        ${SDL2_DIR}/lib
        $ENV{SDL2_DIR}/lib/x64
        $ENV{SDL2_DIR}/lib
    )
    
elseif(APPLE)
    find_path(SDL2_INCLUDE_DIR SDL.h
        HINTS 
        /opt/homebrew/include/SDL2
        /usr/local/include/SDL2
    )
    
    find_library(SDL2_LIBRARY SDL2
        HINTS 
        /opt/homebrew/lib
        /usr/local/lib
    )
    
else()
    # Linux: Use pkg-config
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL2 sdl2)
    endif()
    
    if(SDL2_FOUND)
        set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
        set(SDL2_LIBRARY ${SDL2_LIBRARIES})
    else()
        find_path(SDL2_INCLUDE_DIR SDL.h
            HINTS /usr/include/SDL2 /usr/local/include/SDL2
        )
        find_library(SDL2_LIBRARY SDL2
            HINTS /usr/lib /usr/local/lib
        )
    endif()
endif()

# If SDL2 not found and auto-install is enabled, install it
if((NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY) AND AUTO_INSTALL_SDL2)
    install_sdl2()
    
    # Re-search for SDL2 after installation
    if(WIN32)
        find_path(SDL2_INCLUDE_DIR SDL.h HINTS ${SDL2_DIR}/include)
        find_library(SDL2_LIBRARY SDL2 HINTS ${SDL2_DIR}/lib/x64 ${SDL2_DIR}/lib)
        find_library(SDL2MAIN_LIBRARY SDL2main HINTS ${SDL2_DIR}/lib/x64 ${SDL2_DIR}/lib)
    elseif(APPLE)
        find_path(SDL2_INCLUDE_DIR SDL.h HINTS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
        find_library(SDL2_LIBRARY SDL2 HINTS /opt/homebrew/lib /usr/local/lib)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 REQUIRED sdl2)
        set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
        set(SDL2_LIBRARY ${SDL2_LIBRARIES})
    endif()
endif()

# Final verification
if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 still not found after installation attempt. Please install SDL2 manually.")
endif()

message(STATUS "SDL2 Include: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2 Library: ${SDL2_LIBRARY}")

include_directories(${SDL2_INCLUDE_DIR})

# Source files
file(GLOB SOURCES "src/*.cpp")

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create executable
add_executable(PingPongEngine ${SOURCES})

# Link libraries (platform-specific)
if(WIN32)
    target_link_libraries(PingPongEngine ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})
    
    # Copy SDL2.dll to output directory on Windows
    if(EXISTS "${SDL2_DIR}/lib/x64/SDL2.dll")
        add_custom_command(TARGET PingPongEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/lib/x64/SDL2.dll"
            $<TARGET_FILE_DIR:PingPongEngine>
        )
    endif()
else()
    target_link_libraries(PingPongEngine ${SDL2_LIBRARY})
endif()

# Custom run target
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PingPongEngine
    DEPENDS PingPongEngine
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)




# First version working on mac
# cmake_minimum_required(VERSION 3.15)
# project(PingPongEngine)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Find SDL2 manually (for macOS/Homebrew)
# find_path(SDL2_INCLUDE_DIR SDL.h
#     HINTS /opt/homebrew/include/SDL2 /usr/local/include/SDL2
# )
# find_library(SDL2_LIBRARY SDL2
#     HINTS /opt/homebrew/lib /usr/local/lib
# )
# include_directories(${SDL2_INCLUDE_DIR})

# # Source files
# file(GLOB SOURCES "src/*.cpp")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# add_executable(PingPongEngine ${SOURCES})
# target_link_libraries(PingPongEngine ${SDL2_LIBRARY})

# add_custom_target(run
#     COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PingPongEngine
#     DEPENDS PingPongEngine
#     WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
# )