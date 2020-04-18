#
# Copyright (C) 2018-2020 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.1)

project(Patronum LANGUAGES CXX)
add_subdirectory(qt-solutions/qtservice)

include(qt-solutions/CMake/ProjectOut.cmake)
include(qt-solutions/CMake/Version.cmake)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DPATRONUM_LIBRARY)

find_package(Qt5 COMPONENTS Core REQUIRED)

file(GLOB SOURCE_CPP
    "src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_CPP})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt-Service)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

setVersion(0 1 0)
