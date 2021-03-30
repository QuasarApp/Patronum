#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.10)
project(Patronum LANGUAGES CXX)
if(TARGET ${PROJECT_NAME})
  message("The ${PROJECT_NAME} arledy included in main Project")
  return()
endif()

if(ANDROID)
    message("The ${PROJECT_NAME} is still not supports Android builds  :(")
    return()
endif()

include(Patronum/QuasarAppLib/CMake/ccache.cmake)
# Add sub directories
add_subdirectory(Patronum)

if (NOT ${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
    add_subdirectory(Tests)
endif()

include(Patronum/QuasarAppLib/CMake/QuasarAppCITargets.cmake)
initAll()

addDoc("Patronum" ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)

