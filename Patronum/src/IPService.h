/*
 * Copyright (C) 2018-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ISERVICE_H
#define ISERVICE_H

#include <QList>
#include "Patronum_global.h"

namespace Patronum {

class Feature;

/**
 * @brief The IService class is base interface of the service.
 */
class PATRONUM_LIBRARYSHARED_EXPORT IService
{
public:
    IService();
    virtual ~IService() = default;

    /**
     * @brief handleReceiveData This method get all received commnads and process its.
     *  For each command will invoke the IService::handleReceive method.
     * @param data This is array of the incomming requests (commands)
     */
    virtual void handleReceiveData(const QSet<Feature>& data) = 0;

    /**
     * @brief handleReceive This method invoked when service receive a request from terminal.
     *  Override this method for work your service.
     * @param data This is input data.
     * @return This method should be return true if the @a data command is supported and processed successful.
     * IF you return false then a negative message will be sent to a terminal app.
     */
    virtual bool handleReceive(const Feature &data) = 0;

    /**
     * @brief supportedFeatures This method should be return the list of the supported fetures.
     *  Override this method for add your features for the service.
     * @return should be return a set of supported features.
     */
    virtual QSet<Feature> supportedFeatures() = 0;

    /**
     * @brief onStart This method invoked when service is started successful.
     * @return true if service started successful
     */
    virtual bool onStart() = 0;

    /**
     * @brief onStop This method invoked when service receive stop command from the terminal.
     */
    virtual void onStop() = 0;

    /**
     * @brief onResume This method invoked when service receive resume command from the terminal.
     */
    virtual void onResume() = 0;

    /**
     * @brief onPause This method invoked when service receive pause command from the terminal.
     */
    virtual void onPause() = 0;
};
}
#endif // ISERVICE_H
