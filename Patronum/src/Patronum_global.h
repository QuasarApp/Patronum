/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PATRONUM_GLOBAL_H
#define PATRONUM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PATRONUM_LIBRARY)
#  define PATRONUM_LIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PATRONUM_LIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Patronum {


/**
 * @brief The PatronumError enum - controller work error codes
 */
enum PatronumError {
    /// Unknown error
    Undefined,
    /// Service is unavailable. Try send start command or restart the service manually.
    ServiceUnavailable,
    /// Invalid package received.
    InvalidPackage,
    /// Library unsupported command received.
    WrongCommand,
    /// Timeout error. service unavailable or not started.
    TimeOutError,
    /// Internal error of the work of the Patronum library. Contact the developers and provide them with an error report. https://github.com/QuasarApp/Patronum/issues
    SystemError,
    /// This error ocured when the socket file alredy created or application do not have a permision to the /var/tmp location.
    /// @note If Preview session of your service is crashed then you need to remove the **/var/tmp/P<ServiceName>** file befor start new session of the service.
    SocketIsBusy,
    /// This error ocured when service not supportded using platform.
    UnsupportedPlatform

};

/**
 * @brief errorToString This method convert the PatronumError to QString.
 * @param error This is error id.
 * @return return text of error.
 */
QString errorToString(PatronumError error);

}
#endif // PATRONUM_GLOBAL_H
