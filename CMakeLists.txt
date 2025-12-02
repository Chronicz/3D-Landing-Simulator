cmake_minimum_required(VERSION 3.15)
project(3D-Landing-Simulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find openFrameworks
# Adjust this path to your openFrameworks installation
set(OF_PATH "$ENV{USERPROFILE}/Documents/openFrameworks")
if(NOT EXISTS "${OF_PATH}")
    set(OF_PATH "C:/openFrameworks")
endif()

if(NOT EXISTS "${OF_PATH}")
    message(FATAL_ERROR "openFrameworks not found. Please set OF_PATH or install openFrameworks.")
endif()

message(STATUS "Using openFrameworks path: ${OF_PATH}")

# Include openFrameworks
include_directories(
    ${OF_PATH}/libs/openFrameworks
    ${OF_PATH}/libs/openFrameworks/utils
    ${OF_PATH}/libs/openFrameworks/3d
    ${OF_PATH}/libs/openFrameworks/gl
    ${OF_PATH}/libs/openFrameworks/graphics
    ${OF_PATH}/libs/openFrameworks/math
    ${OF_PATH}/libs/openFrameworks/sound
    ${OF_PATH}/libs/openFrameworks/app
    ${OF_PATH}/libs/openFrameworks/communication
    ${OF_PATH}/libs/openFrameworks/events
    ${OF_PATH}/libs/openFrameworks/types
    ${OF_PATH}/libs/openFrameworks/video
    ${OF_PATH}/libs/openFrameworks/sound
    ${OF_PATH}/libs/openFrameworks/gl
    ${OF_PATH}/libs/glfw/include
    ${OF_PATH}/libs/glfw/include/GLFW
    ${OF_PATH}/libs/FreeImage/include
    ${OF_PATH}/libs/freetype/include
    ${OF_PATH}/libs/freetype/include/freetype2
    ${OF_PATH}/libs/glm/include
    ${OF_PATH}/libs/kiss/include
    ${OF_PATH}/libs/poco/include
    ${OF_PATH}/libs/rtaudio/include
    ${OF_PATH}/libs/tess2/include
    ${OF_PATH}/libs/utf8/include
    ${OF_PATH}/libs/uriparser/include
    ${OF_PATH}/addons
)

# Addons
set(ADDONS_DIR "${OF_PATH}/addons")
include_directories(
    ${ADDONS_DIR}/ofxGui/src
    ${ADDONS_DIR}/ofxAssimpModelLoader/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/ofApp.cpp
    src/Octree.cpp
    src/Util.cpp
    src/box.cc
)

# Create executable
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# Link directories
link_directories(
    ${OF_PATH}/libs/openFrameworksCompiled/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/glfw/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/FreeImage/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/freetype/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/poco/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/rtaudio/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/tess2/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${OF_PATH}/libs/uriparser/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${ADDONS_DIR}/ofxGui/lib/vs/${CMAKE_VS_PLATFORM_NAME}
    ${ADDONS_DIR}/ofxAssimpModelLoader/lib/vs/${CMAKE_VS_PLATFORM_NAME}
)

# Libraries to link
target_link_libraries(${PROJECT_NAME}
    openFrameworks
    ofxGui
    ofxAssimpModelLoader
    glfw3
    freetype
    FreeImage
    rtaudio
    tess2
    uriparser
    opengl32
    glu32
    winmm
    ws2_32
    wsock32
)

# Copy data files
file(COPY ${CMAKE_SOURCE_DIR}/moonterrain_finalproject.obj 
         ${CMAKE_SOURCE_DIR}/moonterrain_finalproject.mtl
         ${CMAKE_SOURCE_DIR}/moonterrain(enl)_finalproject.obj
         ${CMAKE_SOURCE_DIR}/moonterrain(enl)_finalproject.mtl
     DESTINATION ${CMAKE_BINARY_DIR})

# Create geo directory and copy files there
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/geo)
file(COPY ${CMAKE_SOURCE_DIR}/moonterrain_finalproject.obj 
         ${CMAKE_SOURCE_DIR}/moonterrain_finalproject.mtl
         ${CMAKE_SOURCE_DIR}/moonterrain(enl)_finalproject.obj
         ${CMAKE_SOURCE_DIR}/moonterrain(enl)_finalproject.mtl
     DESTINATION ${CMAKE_BINARY_DIR}/geo)

