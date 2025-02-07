#
# Copyright (C) 2018-2025 QuasarApp.
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


include(submodules/QuasarAppLib/CMake/QuasarApp.cmake)
# Add sub directories

if (IOS)
    message("The ios not support QProcess class.")
    initAll()
    return()
endif()

add_subdirectory(submodules/QuasarAppLib)

add_subdirectory(Patronum)

if (NOT DEFINED PATRONUM_TESTS)
    set(PATRONUM_TESTS ON)

    if (DEFINED TARGET_PLATFORM_TOOLCHAIN)
        if (${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
            set(PATRONUM_TESTS OFF)
        endif()
    endif()

    if (ANDROID OR IOS)
        set(PATRONUM_TESTS OFF)
    endif()
endif()


if (PATRONUM_TESTS)
    add_subdirectory(Tests)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()

initAll()

addDoc("Patronum" ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)

