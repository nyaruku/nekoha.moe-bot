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

set(ENABLE_MYSQL ON)
add_definitions(-DORMPP_ENABLE_MYSQL)

set(sioclient "${CMAKE_SOURCE_DIR}/src/libs/libsioclient.a")

# STATIC LIB, you are a stupid fuck if u dont
option(USE_STATIC_LIB "Use static library" ON)

find_package(DPP REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

find_path(MYSQL_INCLUDE_DIR mysql.h PATH_SUFFIXES mysql)
find_library(MYSQL_LIBRARIES mysqlclient)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    dpp
    ${sioclient}
    ${MYSQL_LIBRARIES}
    z       #zlib
    Threads::Threads
    OpenSSL::Crypto
    OpenSSL::SSL
)

# Debug/Release-specific paths
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/includes/")
    set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/src/libs/")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DPP_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
)

# Add the include directories for the project
include_directories(${BASE_INCLUDE_PATH} ${BUILD_INCLUDE_PATH})

# Add the library directories for linking
link_directories(${BASE_LIB_PATH} ${BUILD_LIB_PATH})