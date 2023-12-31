/*
 * Copyright (C) 2018-2024 QuasarApp.
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
 * @brief The IController class This is base interface for the handling network events.
 *
 */
class PATRONUM_LIBRARYSHARED_EXPORT IController
{
public:
    IController() = default;
    virtual ~IController() = default;

    /**
     * @brief handleFeatures This method should be handle all Command::Features resuests.
     *  This method will invoked when controller receive commands list for execute (features) from controller or terminal.
     * @param features This is list of the requests (@a features)
     */
    virtual void handleFeatures(const QList<Feature>& features) = 0;

    /**
     * @brief handleResponce This method should be handle all responces of the service.
     *  This method will invoked when controller receive responce from service.
     * @param responce This is responce message.
     */
    virtual void handleResponce(const QVariantMap& responce) = 0;

    /**
     * @brief handleError This method shold be handle all error messages.
     * This method will invoked when a controlee receive a error responce from a service.
     * @param errorCode This is code of a error.
     */
    virtual void handleError(PatronumError errorCode) = 0;

    /**
     * @brief finished This method ivoked when controler received Command::CloseConnection from the server.
     */
    virtual void finished() = 0;

};
}

#endif // ICONTROLER_H
