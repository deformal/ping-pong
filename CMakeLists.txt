set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "Starting CMake configuration...")
cmake_minimum_required(VERSION 3.15)
project(PingPongEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(AUTO_INSTALL_SDL2 "Automatically install SDL2 if not found" ON)
option(AUTO_INSTALL_SDL2_TTF "Automatically install SDL2_ttf if not found" ON)

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
        message(STATUS "Downloading SDL2 framework for macOS...")
        set(SDL2_VERSION "2.28.5")
        set(SDL2_URL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.dmg")
        set(SDL2_DMG "${CMAKE_BINARY_DIR}/SDL2.dmg")
        set(SDL2_EXTRACT_DIR "${CMAKE_SOURCE_DIR}/SDL2")
        
        file(DOWNLOAD ${SDL2_URL} ${SDL2_DMG}
            SHOW_PROGRESS
            STATUS DOWNLOAD_STATUS
        )
        
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download SDL2")
        endif()
        
        execute_process(
            COMMAND hdiutil attach ${SDL2_DMG} -mountpoint ${CMAKE_BINARY_DIR}/SDL2_mount
            RESULT_VARIABLE MOUNT_RESULT
            OUTPUT_VARIABLE MOUNT_OUTPUT
            ERROR_VARIABLE MOUNT_ERROR
        )
        
        message(STATUS "Mount result: ${MOUNT_RESULT}")
        message(STATUS "Mount output: ${MOUNT_OUTPUT}")
        message(STATUS "Mount error: ${MOUNT_ERROR}")
        
        if(NOT MOUNT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to mount SDL2 DMG: ${MOUNT_ERROR}")
        endif()
        
        file(MAKE_DIRECTORY ${SDL2_EXTRACT_DIR})
        execute_process(
            COMMAND cp -R ${CMAKE_BINARY_DIR}/SDL2_mount/SDL2.framework ${SDL2_EXTRACT_DIR}/
            RESULT_VARIABLE COPY_RESULT
            OUTPUT_VARIABLE COPY_OUTPUT
            ERROR_VARIABLE COPY_ERROR
        )
        
        message(STATUS "Copy result: ${COPY_RESULT}")
        message(STATUS "Copy output: ${COPY_OUTPUT}")
        message(STATUS "Copy error: ${COPY_ERROR}")
        
        execute_process(
            COMMAND hdiutil detach ${CMAKE_BINARY_DIR}/SDL2_mount
            OUTPUT_VARIABLE DETACH_OUTPUT
            ERROR_VARIABLE DETACH_ERROR
        )
        
        message(STATUS "Detach output: ${DETACH_OUTPUT}")
        
        file(REMOVE ${SDL2_DMG})
        
        if(NOT COPY_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to copy SDL2 framework")
        endif()
        
        set(SDL2_DIR ${SDL2_EXTRACT_DIR} PARENT_SCOPE)
        message(STATUS "SDL2 framework installed to ${SDL2_EXTRACT_DIR}/SDL2.framework")
    endif()
endfunction()

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
        message(STATUS "Downloading SDL2_ttf framework for macOS...")
        set(SDL2_TTF_VERSION "2.22.0")
        set(SDL2_TTF_URL "https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/SDL2_ttf-${SDL2_TTF_VERSION}.dmg")
        set(SDL2_TTF_DMG "${CMAKE_BINARY_DIR}/SDL2_ttf.dmg")
        set(SDL2_TTF_EXTRACT_DIR "${CMAKE_SOURCE_DIR}/SDL2_ttf")
        
        file(DOWNLOAD ${SDL2_TTF_URL} ${SDL2_TTF_DMG}
            SHOW_PROGRESS
            STATUS DOWNLOAD_STATUS
        )
        
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download SDL2_ttf")
        endif()
        
        execute_process(
            COMMAND hdiutil attach ${SDL2_TTF_DMG} -mountpoint ${CMAKE_BINARY_DIR}/SDL2_ttf_mount
            RESULT_VARIABLE MOUNT_RESULT
            OUTPUT_VARIABLE MOUNT_OUTPUT
            ERROR_VARIABLE MOUNT_ERROR
        )
        
        message(STATUS "Mount result: ${MOUNT_RESULT}")
        message(STATUS "Mount output: ${MOUNT_OUTPUT}")
        message(STATUS "Mount error: ${MOUNT_ERROR}")
        
        if(NOT MOUNT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to mount SDL2_ttf DMG: ${MOUNT_ERROR}")
        endif()
        
        file(MAKE_DIRECTORY ${SDL2_TTF_EXTRACT_DIR})
        execute_process(
            COMMAND cp -R ${CMAKE_BINARY_DIR}/SDL2_ttf_mount/SDL2_ttf.framework ${SDL2_TTF_EXTRACT_DIR}/
            RESULT_VARIABLE COPY_RESULT
            OUTPUT_VARIABLE COPY_OUTPUT
            ERROR_VARIABLE COPY_ERROR
        )
        
        message(STATUS "Copy result: ${COPY_RESULT}")
        message(STATUS "Copy output: ${COPY_OUTPUT}")
        message(STATUS "Copy error: ${COPY_ERROR}")
        
        execute_process(
            COMMAND hdiutil detach ${CMAKE_BINARY_DIR}/SDL2_ttf_mount
            OUTPUT_VARIABLE DETACH_OUTPUT
            ERROR_VARIABLE DETACH_ERROR
        )
        
        message(STATUS "Detach output: ${DETACH_OUTPUT}")
        
        file(REMOVE ${SDL2_TTF_DMG})
        
        if(NOT COPY_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to copy SDL2_ttf framework")
        endif()
        
        set(SDL2_TTF_DIR ${SDL2_TTF_EXTRACT_DIR} PARENT_SCOPE)
        message(STATUS "SDL2_ttf framework installed to ${SDL2_TTF_EXTRACT_DIR}/SDL2_ttf.framework")
    endif()
endfunction()

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2" CACHE PATH "Path to SDL2 directory")

if(WIN32)
    find_path(SDL2_INCLUDE_DIR SDL.h
        HINTS 
        ${SDL2_DIR}/include
        $ENV{SDL2_DIR}/include
        NO_DEFAULT_PATH
    )
    
    find_library(SDL2_LIBRARY 
        NAMES SDL2
        HINTS 
        ${SDL2_DIR}/lib/x64
        ${SDL2_DIR}/lib
        $ENV{SDL2_DIR}/lib/x64
        $ENV{SDL2_DIR}/lib
        NO_DEFAULT_PATH
    )
    
    find_library(SDL2MAIN_LIBRARY 
        NAMES SDL2main
        HINTS 
        ${SDL2_DIR}/lib/x64
        ${SDL2_DIR}/lib
        $ENV{SDL2_DIR}/lib/x64
        $ENV{SDL2_DIR}/lib
        NO_DEFAULT_PATH
    )
    
elseif(APPLE)
    if(EXISTS "${SDL2_DIR}/SDL2.framework")
        set(SDL2_FRAMEWORK "${SDL2_DIR}/SDL2.framework")
        set(SDL2_INCLUDE_DIR "${SDL2_FRAMEWORK}/Headers")
        set(SDL2_LIBRARY "${SDL2_FRAMEWORK}")
        message(STATUS "Found local SDL2 framework at ${SDL2_FRAMEWORK}")
    else()
        set(SDL2_INCLUDE_DIR "")
        set(SDL2_LIBRARY "")
    endif()
endif()

if((NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY) AND AUTO_INSTALL_SDL2)
    install_sdl2()
    
    if(WIN32)
        find_path(SDL2_INCLUDE_DIR SDL.h HINTS ${SDL2_DIR}/include NO_DEFAULT_PATH)
        find_library(SDL2_LIBRARY SDL2 HINTS ${SDL2_DIR}/lib/x64 ${SDL2_DIR}/lib NO_DEFAULT_PATH)
        find_library(SDL2MAIN_LIBRARY SDL2main HINTS ${SDL2_DIR}/lib/x64 ${SDL2_DIR}/lib NO_DEFAULT_PATH)
    elseif(APPLE)
        set(SDL2_FRAMEWORK "${SDL2_DIR}/SDL2.framework")
        set(SDL2_INCLUDE_DIR "${SDL2_FRAMEWORK}/Headers")
        set(SDL2_LIBRARY "${SDL2_FRAMEWORK}")
    endif()
endif()

if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 still not found after installation attempt. Please install SDL2 manually.")
endif()

message(STATUS "SDL2 Include: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2 Library: ${SDL2_LIBRARY}")

set(SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/SDL2_ttf" CACHE PATH "Path to SDL2_ttf directory")

if(WIN32)
    find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS 
        ${SDL2_TTF_DIR}/include
        $ENV{SDL2_TTF_DIR}/include
        NO_DEFAULT_PATH
    )
    
    find_library(SDL2_TTF_LIBRARY 
        NAMES SDL2_ttf
        HINTS 
        ${SDL2_TTF_DIR}/lib/x64
        ${SDL2_TTF_DIR}/lib
        $ENV{SDL2_TTF_DIR}/lib/x64
        $ENV{SDL2_TTF_DIR}/lib
        NO_DEFAULT_PATH
    )
    
elseif(APPLE)
    if(EXISTS "${SDL2_TTF_DIR}/SDL2_ttf.framework")
        set(SDL2_TTF_FRAMEWORK "${SDL2_TTF_DIR}/SDL2_ttf.framework")
        set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_FRAMEWORK}/Headers")
        set(SDL2_TTF_LIBRARY "${SDL2_TTF_FRAMEWORK}")
        message(STATUS "Found local SDL2_ttf framework at ${SDL2_TTF_FRAMEWORK}")
    else()
        set(SDL2_TTF_INCLUDE_DIR "")
        set(SDL2_TTF_LIBRARY "")
    endif()
endif()

if((NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY) AND AUTO_INSTALL_SDL2_TTF)
    install_sdl2_ttf()
    
    if(WIN32)
        find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h HINTS ${SDL2_TTF_DIR}/include NO_DEFAULT_PATH)
        find_library(SDL2_TTF_LIBRARY SDL2_ttf HINTS ${SDL2_TTF_DIR}/lib/x64 ${SDL2_TTF_DIR}/lib NO_DEFAULT_PATH)
    elseif(APPLE)
        set(SDL2_TTF_FRAMEWORK "${SDL2_TTF_DIR}/SDL2_ttf.framework")
        set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_FRAMEWORK}/Headers")
        set(SDL2_TTF_LIBRARY "${SDL2_TTF_FRAMEWORK}")
    endif()
endif()

if(NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_ttf still not found after installation attempt. Please install SDL2_ttf manually.")
endif()

message(STATUS "SDL2_ttf Include: ${SDL2_TTF_INCLUDE_DIR}")
message(STATUS "SDL2_ttf Library: ${SDL2_TTF_LIBRARY}")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(PingPongEngine ${SOURCES})

if(WIN32)
    target_link_libraries(PingPongEngine ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    
    if(EXISTS "${SDL2_DIR}/lib/x64/SDL2.dll")
        add_custom_command(TARGET PingPongEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/lib/x64/SDL2.dll"
            $<TARGET_FILE_DIR:PingPongEngine>
        )
    endif()
    
    if(EXISTS "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll")
        add_custom_command(TARGET PingPongEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:PingPongEngine>
        )
    endif()
elseif(APPLE)
    target_link_libraries(PingPongEngine ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/SDL2/SDL2.framework")
        set_target_properties(PingPongEngine PROPERTIES
            BUILD_RPATH "${CMAKE_SOURCE_DIR}/SDL2;${CMAKE_SOURCE_DIR}/SDL2_ttf"
            INSTALL_RPATH "@executable_path/../Frameworks"
        )
    endif()
endif()

message("Unzipping fonts")
execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/assets/arcadeclassic.zip
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets 
)

add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PingPongEngine
    DEPENDS PingPongEngine
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_command(TARGET PingPongEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copying assets for PingPongEngine"
)