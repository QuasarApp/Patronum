/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "IPController.h"

QString Patronum::IController::errorToString(Patronum::ControllerError error) const {
    switch (error) {
    case Patronum::ControllerError::ServiceUnavailable:
        return QObject::tr("Service is unavailable. Try send start comand or restart the service manually.");

    case Patronum::ControllerError::InvalidPackage:
        return QObject::tr("Invalid package received");

    case Patronum::ControllerError::WrongCommand:
        return QObject::tr("Library unsupported command received");

    case Patronum::ControllerError::SystemError:
        return QObject::tr("Internal error of the work of the Patronum library."
                           " Contact the developers and provide them with an error report."
                           " https://github.com/QuasarApp/Patronum/issues");

    case Patronum::ControllerError::TimeOutError:
        return QObject::tr("Timeout error. service unavailable or not started.");

    default:
        return QObject::tr("Unknown error");
    }
}
