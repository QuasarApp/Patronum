/*
 * Copyright (C) 2018-2020 QuasarApp.
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
     * @param argc - count params
     * @param argv - test of arguments
     * @param name - name of your service
     */
    ServiceBase(int argc, char *argv[], const QString &name);

    ~ServiceBase() override;
    // IService interface

    /**
     * @brief exec
     * @return result of work application
     */
    virtual int exec();

protected:
    /**
     * @brief handleReceive - this method invoce when service receive new command from terminal of controller of this service
     * @param data - is list of commands from controller
     * Default inplementation send message abount error.
     */
    void handleReceive(const QList<Feature> &data) override;

    /**
     * @brief supportedFeatures
     * @return list of supported features of this service. override this method for correctly work of your pair (service and controller)
     */
    QList<Feature> supportedFeatures() override;

    /**
     * @brief sendResuylt - call this method for send responce from service to tour controller
     * @param result
     * @return true if data sendet is seccusseful
     */
    bool sendResuylt(const QVariantMap &result);

    /**
     * @brief sendResuylt this method send text responce to controller
     * @param result - message
     * @return true if data sendet is seccusseful
     */
    bool sendResuylt(const QString &result);

    /**
     * @brief createApplication default implementation create a Application object and parse argumnts.
     */
    virtual void createApplication() = 0;

    /**
     * @brief onStart called when get start command from terminal. Override this method work correctly work of service.
     */
    void onStart() override = 0;

    /**
     * @brief onStop called when get stop command from terminal. The default implementation of this method invoke a quit method of QCoreApplication.
     */
    void onStop() override;

    /**
     * @brief onStop called when get resume command from terminal. The Default implementation do nothing.
     */
    void onResume() override;

    /**
     * @brief onStop called when get pause command from terminal.  The Default implementation do nothing.
     */
    void onPause() override;

    /**
     * @brief controller
     * @return own controller instance;
     */
    Controller *controller() const;

    QCoreApplication *_core = nullptr;
private:

    ServicePrivate *d_ptr = nullptr;
    Controller *_controller = nullptr;
    QString _serviceName = "";

};

}


#endif // SERVICE_BASE_H
