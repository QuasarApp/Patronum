/*
 * Copyright (C) 2018-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Patronum_global.h"
#include <IPController.h>
#include "PFeature.h"
#include <quasarapp.h>

namespace Patronum {

class ControllerPrivate;

/**
 * @brief The Controller class provide control functionality for your service.
 *
 * ### How to use :
 * - create QCoreApplication object
 * - just inherit from the Service Controller and override the methods you need.
 * - So, invoke the send method, and if you need to get a response from your service then invoke a waitForResponce method.
 * - or run application exec method.
 *
 * ### Example:
 *
 * @code{cpp}
    #include <patronum.h>

    class MyControllerApp : public Patronum::Controller
    {
    public:

        MyControllerApp():
            Patronum::Controller() {

        }
    };

    int main(int argc, char **argv) {
        QCoreApplication::setApplicationName("MyServiceName"); // <--
        QCoreApplication::setOrganizationName("MyCompany"); // <--
        QCoreApplcication app
        MyControllerApp controller;
        controller.send(argc, argv);
        return app.exec();
    }
 * @endcode
 *
 * @note The Controller class provide only base functionality. Sending custom command and receive response of service. If you want to start or install/unistall service tou need to use the Patronum::Service class.
 *
 */
class PATRONUM_LIBRARYSHARED_EXPORT Controller : protected IController
{
public:
    /**
     * @brief Controller This is base constructor of the controller.
     */
    Controller();
    ~Controller() override;

    /**
     * @brief send This method send request to service.
     * @param argc This is count of arguments.
     * @param argv This is arguments list.
     * @return true if all sendet successful.
     */
    bool send(int argc, char **argv);

    /**
     * @brief send - This method send request to service.
     * @warning Invoke this method if you invoked QuasarAppUtils::Params::parse() before invoke this method else use send(int argc, char **argv).
     * @return true if all sendet successful.
     */
    bool send();

    /**
     * @brief sendStop This method send stop command to service;
     * @return trie if command sendet successfull
     */
    bool sendStop();

    /**
     * @brief help This method return help of the Controller.
     * @return Available otions list.
     */
    QuasarAppUtils::Help::Section help() const;

    // IControler interface
protected:

    /**
     * @brief handleError - override this method if you want track errors
        the default implementation print error message.
       @param error - error id see PatronumError
     */
    void handleError(PatronumError error) override;

    /**
     * @brief handleFeatures - Override this method if you want create a custom reaction of get service features.
     * default implementation print help of available command of your service.
     * @param features - List of features.
     */
    void handleFeatures(const QList<Feature> &features) override;

    /**
     * @brief handleResponce - Override this method if you want create a custom reaction of get responce from service.
     * Default inplementation print responce to console.
     * @param responce - Responce from service.
     */
    void handleResponce(const QVariantMap &responce) override;

    /**
     * @brief finished This method invoked when controler receive from service the Command::CloseConnection command
     * This implementation invoke he exit method of the QCoreApplication and finished application.
     *  If do not want to stop application after receive Command::CloseConnection then override this method.
     */
    void finished() override;

    /**
     * @brief features - This method return current features of connected service.
     * @note If Respond from service not received then return empty list.
     * @return Features list.
     */
    QList<Feature> features();


private:
    ControllerPrivate *d_ptr = nullptr;
    bool _disableFinished = false;
    void printDefaultHelp() const;
};
}
#endif // CONTROLLER_H
