/*
 * Copyright (C) 2018-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "Patronum_global.h"
#include <QObject>

namespace Patronum {

QString errorToString(PatronumError error) {
    switch (error) {
    case Patronum::PatronumError::ServiceUnavailable:
        return QObject::tr("Service is unavailable. Try send start comand or restart the service manually.");

    case Patronum::PatronumError::InvalidPackage:
        return QObject::tr("Invalid package received");

    case Patronum::PatronumError::WrongCommand:
        return QObject::tr("Library unsupported command received");

    case Patronum::PatronumError::SystemError:
        return QObject::tr("Internal error of the work of the Patronum library."
                           " Contact the developers and provide them with an error report."
                           " https://github.com/QuasarApp/Patronum/issues");

    case Patronum::PatronumError::TimeOutError:
        return QObject::tr("Timeout error. service unavailable or not started.");

    case Patronum::PatronumError::FailedToStart:
        return QObject::tr("Failed to start service");

    case Patronum::PatronumError::UnsupportedPlatform:
        return QObject::tr("The service not supportded using platform.");
    default:
        return QObject::tr("Unknown error");
    }
}

}
