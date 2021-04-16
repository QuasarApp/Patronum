/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Patronum_global.h"
#include <IPController.h>
#include "PFeature.h"

namespace Patronum {

class ControllerPrivate;

/**
 * @brief The Controller class provide control functionality for your service
 * ###How to use :
 * - create QCoreApplication object
 * - just inherit from the Service Controller and override the methods you need.
 * - So, invoke the send methon, and if you need to get a responce from your service then invoke a waitForResponce method.
 * - or run application exec method.
 */
class PATRONUM_LIBRARYSHARED_EXPORT Controller : protected IController
{
public:
    /**
     * @brief Controller - base constructor
     * @param name - name of you service
     * @param servicePath - path to service executable. @note If servicePath argument well be empty then 'start' commnad not working
     */
    Controller(const QString& name, const QString& servicePath = "");
    ~Controller() override;

    /**
     * @brief send - this method send request to service
     * @param argc - count of arguments
     * @param argv - arguments list
     * @return true if all sendet seccussful
     */
    bool send(int argc, char **argv);

    /**
     * @brief send - this method send request to service. @warning Invoke this method if you invoked QuasarAppUtils::Params::parse() befor invoke this method else use send(int argc, char **argv).
     * @return true if all sendet seccussful
     */
    bool send();

    /**
     * @brief startDetached
     * @return true if service started seccessful.
     */
    int startDetached() const;

    /**
     * @brief waitForResponce - waut for get a responce from servece
     * @param msec timeout
     * @return true if all seccussful
     */
    bool waitForResponce(int msec = 10000);

    // IControler interface
protected:

    /**
     * @brief handleError - override this method if you want track errors
        the default implementation print error message.
       @param error - error id see ControllerError
     */
    void handleError(ControllerError error) override;

    /**
     * @brief handleFeatures - override this method if you want cerate a custom reaction of get service features
     * default inplenebtation prin help of available command of your service
     * @param features - list of features
     */
    void handleFeatures(const QList<Feature> &features) override;

    /**
     * @brief handleResponce - override this method if you want create a custom reaction of get responce from service
     * Default inplementation print responce to console.
     * @param responce - responce from service
     */
    void handleResponce(const QVariantMap &responce) override;

    /**
     * @brief features - this metho return current features of connected service.
     * @note If Responed from service not received then return empty list.
     * @return features list
     */
    QList<Feature> features();


private:
    ControllerPrivate *d_ptr = nullptr;

    void printDefaultHelp() const;


};
}
#endif // CONTROLLER_H
