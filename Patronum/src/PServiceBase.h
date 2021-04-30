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
 * @brief The ServiceBase class This is Base service class. Please for create your own services use the Patronum::Service class.
 */
class PATRONUM_LIBRARYSHARED_EXPORT ServiceBase : protected IService
{
public:
    /**
     * @brief ServiceBase This is main constructor of the service.
     * @param argc This is count of input arguments.
     * @param argv This is raw C array of the input arguments.
     * @note If you create a console client for your service then you need to sets some service name as a this. Fot This use the QCoreApplication::setApplicationName() methid. You need to invoke this method befor invoke the ServiceBase::exec method.
     */
    ServiceBase(int argc, char *argv[]);

    ~ServiceBase() override;
    // IService interface

    /**
     * @brief exec This is main method of the service. Use this like a QCoreApplication::exec.
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
     * @brief controller This method return the local controller object. If the controller object is not inited the this methd create a new object.
     * @return Own controller instance.
     */
    Controller *controller();

    /**
     * @brief core This method return a pointer to instance of the core application.
     * @return pointer to core application object.
     */
    QCoreApplication *core();

    /**
     * @brief setCore This method sets new object of the core application.
     * @param core This is new core application object.
     */
    void setCore(QCoreApplication *core);

private:

    void printDefaultHelp();

    ServicePrivate *d_ptr = nullptr;
    Controller *_controller = nullptr;

    /**
     * @brief _core This is point to the main application object.
     */
    QCoreApplication *_core = nullptr;

};

}


#endif // SERVICE_BASE_H
