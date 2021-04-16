/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ICONTROLER_H
#define ICONTROLER_H

#include <QList>
#include <QVariantMap>
#include "Patronum_global.h"

namespace Patronum {

class Feature;

/**
 * @brief The ControllerError enum - controller work error codes
 */
enum class ControllerError {
    /// Unknown error
    Undefined,
    /// Service is unavailable. Try send start command or restart the service manually.
    ServiceUnavailable,
    /// Invalid package received.
    InvalidPackage,
    /// Library unsupported command received.
    WrongCommand,
    /// Internal error of the work of the Patronum library. Contact the developers and provide them with an error report. https://github.com/QuasarApp/Patronum/issues
    SystemError
};

/**
 * @brief The IController class
 */
class PATRONUM_LIBRARYSHARED_EXPORT IController
{
public:
    IController() = default;
    virtual ~IController() = default;

    /**
     * @brief errorToString This method convert the ControllerError to QString.
     * @param error - Error id.
     * @return return text of error.
     */
    QString errorToString(ControllerError error) const;

    virtual void handleFeatures(const QList<Feature>& features) = 0;
    virtual void handleResponce(const QVariantMap& feature) = 0;
    virtual void handleError(ControllerError) = 0;

};
}

#endif // ICONTROLER_H
