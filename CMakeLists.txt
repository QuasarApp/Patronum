#
# Copyright (C) 2018-2020 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.1)
project(PotronumMain)
if(TARGET ${PROJECT_NAME})
  message("The ${PROJECT_NAME} arledy included in main Project")
  return()
endif()

include(Patronum/qt-solutions/CMake/ccache.cmake)
# Add sub directories
add_subdirectory(Patronum)

include(Patronum/qt-solutions/CMake/QuasarAppCITargets.cmake)

initAll()
