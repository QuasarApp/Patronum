/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PATRONUM_H
#define PATRONUM_H

#include "PController.h"
#include "PFeature.h"
#include "PService.h"

/**
 * @brief The Patronum namespace - It is main name space of Patronum Library.
 * The Patronum library support the two work mode
 *  1. Single executable mode.
 *  2. Service-terminal mode.
 *
 *  ## Single executable mode
 *  In this case you have a service with single executable file. All service operations (instalation deinstalation, send commnad and another) will sends from the one executable.
 *
 *  Fits one you need to start service executable as a daemon.
 *  @code{bash}
 *    myservice daemon
 *  @endcode
 *
 *  For send any command to service use the service command.
 *
 *  @code{bash}
 *    myservice myCommand
 *  @endcode
 *
 *  ## Service-terminal mode.
 *  In this mode you create two executable file.
 *  1. Service is Patronum::Service
 *  2. Terminal is Patronum::Controller
 *
 *  Fits one you need to start service executable as a daemon.
 *  @code{bash}
 *    myservice daemon
 *  @endcode
 *
 *  For send any command to service use the terminal executable.
 *
 *  @code{bash}
 *    terminal myCommand
 *  @endcode
 */
namespace Patronum {
/**
 * @brief patronumLibVersion This method return current version of library.
 * @return current version of library
 */
QString patronumLibVersion();
}
#endif // PATRONUM_H
