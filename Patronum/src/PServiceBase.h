/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SERVICE_BASE_H
#define SERVICE_BASE_H
#include "Patronum_global.h"
#include "PFeature.h"
#include <IPService.h>
#include <quasarapp.h>

namespace Patronum {

class ServicePrivate;
class Controller;

/**
 * @brief The ServiceBase class
 */
class PATRONUM_LIBRARYSHARED_EXPORT ServiceBase : protected IService
{
public:
    /**
     * @brief Service
     * @param argc - Count params.
     * @param argv - Test of arguments.
     * @param name - Name of your service.
     */
    ServiceBase(int argc, char *argv[], const QString &name);

    ~ServiceBase() override;
    // IService interface

    /**
     * @brief exec
     * @return Result of work application.
     */
    virtual int exec();

protected:
    /**
     * @brief handleReceiveData - This method invoice when service receive new command from terminal of controller of this service.
     * @param data - Is list of commands from controller.
     * Default implementation send message about error, and invoke the.
     */
    void handleReceiveData(const QSet<Feature> &data) override final;

    /**
     * @brief supportedFeatures
     * @return A set supported features of this service. Override this method for correctly work of your pair (service and controller).
     */
    QSet<Feature> supportedFeatures() override;

    /**
     * @brief sendResuylt - Call this method for send responce from service to tour controller.
     * @param result Message.
     * @return True if data sendet is seccusseful.
     */
    bool sendResuylt(const QVariantMap &result);

    /**
     * @brief sendResuylt This method send text responce to controller.
     * @param result - Message.
     * @return True if data sendet is seccusseful.
     */
    bool sendResuylt(const QString &result);

    /**
     * @brief sendCloseeConnetion This method send signal that all request command processed.
     * @return Return true if the message sent successul.
     */
    bool sendCloseeConnetion();

    /**
     * @brief createApplication Default implementation create a Application object and parse arguments.
     */
    virtual void createApplication() = 0;

    /**
     * @brief onStart Called when get start command from terminal. Override this method work correctly work of service.
     */
    void onStart() override = 0;

    /**
     * @brief onStop Called when get stop command from terminal. The default implementation of this method invoke a quit method of QCoreApplication.
     */
    void onStop() override;

    /**
     * @brief onResume Called when get resume command from terminal. The Default implementation do nothing.
     */
    void onResume() override;

    /**
     * @brief onPause Called when get pause command from terminal. The Default implementation do nothing.
     */
    void onPause() override;

    /**
     * @brief controller
     * @return Own controller instance.
     */
    Controller *controller();

    QCoreApplication *_core = nullptr;
private:


    ServicePrivate *d_ptr = nullptr;
    Controller *_controller = nullptr;
    QString _serviceName = "";

};

}


#endif // SERVICE_BASE_H
