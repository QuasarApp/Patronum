/*
 * Copyright (C) 2018-2027 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QList>
#include <QVariantMap>
#include "Patronum_global.h"

namespace Patronum {

class Feature;

/**
 * @brief The IController class This is base interface for the handling network events.
 *
 */
class PATRONUM_LIBRARYSHARED_EXPORT IController
{
public:
    IController() = default;
    virtual ~IController() = default;

    /**
     * @brief handleFeatures This method should be handle all Command::Features requests.
     *  This method will invoked when controller receive commands list for execute (features) from controller or terminal.
     * @param features This is list of the requests (@a features)
     */
    virtual void handleFeatures(const QList<Feature>& features) = 0;

    /**
     * @brief handleResponse This method should be handle all responses of the service.
     *  This method will invoked when controller receive response from service.
     * @param response This is response message.
     */
    virtual void handleResponse(const QVariantMap& response) = 0;

    /**
     * @brief handleError This method should be handle all error messages.
     * This method will invoked when a controlee receive a error response from a service.
     * @param errorCode This is code of a error.
     */
    virtual void handleError(PatronumError errorCode) = 0;

    /**
     * @brief finished This method invoked when controller received Command::CloseConnection from the server.
     */
    virtual void finished() = 0;

};
}

#endif // ICONTROLLER_H
