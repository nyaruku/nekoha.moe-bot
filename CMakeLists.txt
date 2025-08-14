cmake_minimum_required(VERSION 3.15)
project(nekoha.moe-bot LANGUAGES CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(USE_STATIC_LIB "Use static library" ON)
include(FetchContent)

# Base directories
set(BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/includes")
set(BASE_LIB_PATH "${CMAKE_SOURCE_DIR}/src/libs")

set(sioclient "${CMAKE_SOURCE_DIR}/src/libs/libsioclient.a")

# STATIC LIB, you are a stupid fuck if u dont
option(USE_STATIC_LIB "Use static library" ON)

add_executable(${PROJECT_NAME} src/main.cpp)

find_package(DPP REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    dpp
    ${sioclient}
    ssl     #openssl
    crypto  #libcrypto
    z       #zlib
)

# Debug/Release-specific paths
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/includes/")
    set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/src/libs/")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DPP_INCLUDE_DIR}
)

# Add the include directories for the project
include_directories(${BASE_INCLUDE_PATH} ${BUILD_INCLUDE_PATH})

# Add the library directories for linking
link_directories(${BASE_LIB_PATH} ${BUILD_LIB_PATH})