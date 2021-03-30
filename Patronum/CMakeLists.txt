#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.10)
set(CURRENT_PROJECT ${PROJECT_NAME})

add_subdirectory(QuasarAppLib)

include(QuasarAppLib/CMake/ProjectOut.cmake)
include(QuasarAppLib/CMake/Version.cmake)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DPATRONUM_LIBRARY)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

file(GLOB SOURCE_CPP
    "src/*.cpp"
    "src/Private/*.cpp"
    "src/Private/*.qrc"


)

add_library(${CURRENT_PROJECT} ${SOURCE_CPP})
target_link_libraries(${CURRENT_PROJECT} PUBLIC Qt::Core Qt::Network QuasarApp)
target_include_directories(${CURRENT_PROJECT} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(${CURRENT_PROJECT} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Private")

setVersion(0 2 0)
