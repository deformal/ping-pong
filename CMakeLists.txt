set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "Starting CMake configuration...")
cmake_minimum_required(VERSION 3.15)
project(PingPongEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to enable automatic SDL2 installation
option(AUTO_INSTALL_SDL2 "Automatically install SDL2 if not found" ON)
option(AUTO_INSTALL_SDL2_TTF "Automatically install SDL2_ttf if not found" ON)

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

# Function to install SDL2_ttf on different platforms
function(install_sdl2_ttf)
    message(STATUS "SDL2_ttf not found. Attempting automatic installation...")
    
    if(WIN32)
        message(STATUS "Downloading SDL2_ttf for Windows...")
        set(SDL2_TTF_VERSION "2.22.0")
        set(SDL2_TTF_URL "https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/SDL2_ttf-devel-${SDL2_TTF_VERSION}-VC.zip")
        set(SDL2_TTF_ZIP "${CMAKE_BINARY_DIR}/SDL2_ttf.zip")
        set(SDL2_TTF_EXTRACT_DIR "${CMAKE_SOURCE_DIR}/SDL2_ttf")
        
        file(DOWNLOAD ${SDL2_TTF_URL} ${SDL2_TTF_ZIP}
            SHOW_PROGRESS
            STATUS DOWNLOAD_STATUS
        )
        
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download SDL2_ttf")
        endif()
        
        file(ARCHIVE_EXTRACT INPUT ${SDL2_TTF_ZIP} DESTINATION ${CMAKE_BINARY_DIR}/SDL2_ttf_temp)
        file(GLOB SDL2_TTF_EXTRACTED "${CMAKE_BINARY_DIR}/SDL2_ttf_temp/SDL2_ttf-*")
        file(RENAME ${SDL2_TTF_EXTRACTED} ${SDL2_TTF_EXTRACT_DIR})
        file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/SDL2_ttf_temp")
        file(REMOVE ${SDL2_TTF_ZIP})
        
        set(SDL2_TTF_DIR ${SDL2_TTF_EXTRACT_DIR} PARENT_SCOPE)
        message(STATUS "SDL2_ttf installed to ${SDL2_TTF_EXTRACT_DIR}")
        
    elseif(APPLE)
        message(STATUS "Installing SDL2_ttf via Homebrew...")
        find_program(BREW brew)
        
        if(NOT BREW)
            message(FATAL_ERROR "Homebrew not found. Please install Homebrew from https://brew.sh")
        endif()
        
        execute_process(
            COMMAND ${BREW} install sdl2_ttf
            RESULT_VARIABLE BREW_RESULT
            OUTPUT_VARIABLE BREW_OUTPUT
            ERROR_VARIABLE BREW_ERROR
        )
        
        if(NOT BREW_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to install SDL2_ttf via Homebrew: ${BREW_ERROR}")
        endif()
        
        message(STATUS "SDL2_ttf installed successfully via Homebrew")
        
    else()
        # Linux
        message(STATUS "Attempting to install SDL2_ttf on Linux...")
        
        # Detect package manager
        find_program(APT apt-get)
        find_program(DNF dnf)
        find_program(PACMAN pacman)
        find_program(ZYPPER zypper)
        
        if(APT)
            message(STATUS "Using apt-get to install SDL2_ttf...")
            execute_process(
                COMMAND sudo apt-get update
                COMMAND sudo apt-get install -y libsdl2-ttf-dev
                RESULT_VARIABLE INSTALL_RESULT
            )
        elseif(DNF)
            message(STATUS "Using dnf to install SDL2_ttf...")
            execute_process(
                COMMAND sudo dnf install -y SDL2_ttf-devel
                RESULT_VARIABLE INSTALL_RESULT
            )
        elseif(PACMAN)
            message(STATUS "Using pacman to install SDL2_ttf...")
            execute_process(
                COMMAND sudo pacman -S --noconfirm sdl2_ttf
                RESULT_VARIABLE INSTALL_RESULT
            )
        elseif(ZYPPER)
            message(STATUS "Using zypper to install SDL2_ttf...")
            execute_process(
                COMMAND sudo zypper install -y libSDL2_ttf-devel
                RESULT_VARIABLE INSTALL_RESULT
            )
        else()
            message(FATAL_ERROR "No supported package manager found. Please install SDL2_ttf manually.")
        endif()
        
        if(NOT INSTALL_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to install SDL2_ttf")
        endif()
        
        message(STATUS "SDL2_ttf installed successfully")
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

# Final verification for SDL2
if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 still not found after installation attempt. Please install SDL2 manually.")
endif()

message(STATUS "SDL2 Include: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2 Library: ${SDL2_LIBRARY}")

# Platform-specific SDL2_ttf configuration
if(WIN32)
    set(SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/SDL2_ttf" CACHE PATH "Path to SDL2_ttf directory")
    
    find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS 
        ${SDL2_TTF_DIR}/include
        $ENV{SDL2_TTF_DIR}/include
    )
    
    find_library(SDL2_TTF_LIBRARY 
        NAMES SDL2_ttf
        HINTS 
        ${SDL2_TTF_DIR}/lib/x64
        ${SDL2_TTF_DIR}/lib
        $ENV{SDL2_TTF_DIR}/lib/x64
        $ENV{SDL2_TTF_DIR}/lib
    )
    
elseif(APPLE)
    find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS 
        /opt/homebrew/include/SDL2
        /usr/local/include/SDL2
    )
    
    find_library(SDL2_TTF_LIBRARY SDL2_ttf
        HINTS 
        /opt/homebrew/lib
        /usr/local/lib
    )
    
else()
    # Linux: Use pkg-config
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL2_TTF SDL2_ttf)
    endif()
    
    if(SDL2_TTF_FOUND)
        set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_INCLUDE_DIRS})
        set(SDL2_TTF_LIBRARY ${SDL2_TTF_LIBRARIES})
    else()
        find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
            HINTS /usr/include/SDL2 /usr/local/include/SDL2
        )
        find_library(SDL2_TTF_LIBRARY SDL2_ttf
            HINTS /usr/lib /usr/local/lib
        )
    endif()
endif()

# If SDL2_ttf not found and auto-install is enabled, install it
if((NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY) AND AUTO_INSTALL_SDL2_TTF)
    install_sdl2_ttf()
    
    # Re-search for SDL2_ttf after installation
    if(WIN32)
        find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h HINTS ${SDL2_TTF_DIR}/include)
        find_library(SDL2_TTF_LIBRARY SDL2_ttf HINTS ${SDL2_TTF_DIR}/lib/x64 ${SDL2_TTF_DIR}/lib)
    elseif(APPLE)
        find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h HINTS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
        find_library(SDL2_TTF_LIBRARY SDL2_ttf HINTS /opt/homebrew/lib /usr/local/lib)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
        set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_INCLUDE_DIRS})
        set(SDL2_TTF_LIBRARY ${SDL2_TTF_LIBRARIES})
    endif()
endif()

# Final verification for SDL2_ttf
if(NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_ttf still not found after installation attempt. Please install SDL2_ttf manually.")
endif()

message(STATUS "SDL2_ttf Include: ${SDL2_TTF_INCLUDE_DIR}")
message(STATUS "SDL2_ttf Library: ${SDL2_TTF_LIBRARY}")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

# Source files
file(GLOB SOURCES "src/*.cpp")

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create executable
add_executable(PingPongEngine ${SOURCES})

# Link libraries (platform-specific)
if(WIN32)
    target_link_libraries(PingPongEngine ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    
    # Copy SDL2.dll to output directory on Windows
    if(EXISTS "${SDL2_DIR}/lib/x64/SDL2.dll")
        add_custom_command(TARGET PingPongEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/lib/x64/SDL2.dll"
            $<TARGET_FILE_DIR:PingPongEngine>
        )
    endif()
    
    # Copy SDL2_ttf.dll to output directory on Windows
    if(EXISTS "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll")
        add_custom_command(TARGET PingPongEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:PingPongEngine>
        )
    endif()
else()
    target_link_libraries(PingPongEngine ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
endif()

# Custom run target
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PingPongEngine
    DEPENDS PingPongEngine
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Copy entire assets folder to build dir
add_custom_command(TARGET PingPongEngine POST_BUILD  # ← Use target name here
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copying assets for PingPongEngine"
)