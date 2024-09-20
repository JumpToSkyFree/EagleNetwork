cmake_minimum_required(VERSION 3.2)

project(EagleNetwork VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EAGLE_NET_HEADERS
    # Headers
    include/EagleNetwork/Platform/PlatofrmDefs.hh
    include/EagleNetwork/Result.hh
    include/EagleNetwork/ResrouceInitializer.hh
)

set(EAGLE_NET_SOURCES
    # Sources
    src/main.cpp
    include/EagleNetwork/Platform/PlatofrmDefs.hh
    include/EagleNetwork/Utilities.hh)

add_library(
    ${CMAKE_PROJECT_NAME}
    SHARED
    ${EAGLE_NET_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/include)

add_subdirectory(test)
