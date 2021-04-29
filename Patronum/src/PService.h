/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SERVICE_H
#define SERVICE_H
#include "Patronum_global.h"
#include "PFeature.h"
#include <IPService.h>
#include "PServiceBase.h"
#include <QCoreApplication>
#include <quasarapp.h>

namespace Patronum {

template<class Application>
/**
 * @brief The Service class it is  class for create a services from daemons.
 *
 * ### How to use :
 * - just inherit from the Service class and override the methods you need.
 *
 * ### Example
 *
 * @code{cpp}
    #include <patronum.h>

    class MyserviceApp : public Patronum::Service<QCoreApplication>
    {

    public:
        MyserviceApp(int argc, char **argv):
            Patronum::Service<QCoreApplication>(argc, argv) {

        }

        void start() {
            // call on server started
        }

        void stop() {
            // call on server stoped
        }

        bool HanoiService::handleReceive(const Patronum::Feature &data) {

            if (data.cmd() == "ping") {
                sendResuylt("Pong");
            } else if (data.cmd() == "state") {
                sendResuylt("application status");
            }

            return true;
        }


        QList<Feature> supportedFeatures() {
            QSet<Patronum::Feature> data;

            data << Patronum::Feature("ping", "This is description of the ping command");
            data << Patronum::Feature("state", "return state");

            return data;
        }
    };


    int main(int argc, char **argv) {
        QCoreApplication::setApplicationName("MyServiceName"); // <--
        QCoreApplication::setOrganizationName("MyCompany"); // <--
        MyserviceApp app;
        return app.exec();
    }
 * @endcode
 *
 * @note The Service class use own executable like a main path to service executable.
 *  If your application has custom dependencies and do not work without costom envirement then you need to add **P_RUN_FILE** or **CQT_RUN_FILE** variavle with full path into the your wraper or launcher file.
 *  **Order of the search executable file of the service**
 *  1. P_RUN_FILE variable
 *  2. CQT_RUN_FILE variable
 *  3. Absalute path to executable.
 *
 */
class Service : public ServiceBase
{
public:
    /**
     * @brief Service This is main constructor of the service object.
     * @param argc This is count of input arguments.
     * @param argv This is raw C array with input arguments.
     * @param name This is name of your service.
     * @note If you create a console client for youe service then you need to sets some service name as a this.
     */
    Service(int argc, char *argv[])
        : ServiceBase(argc, argv) {

        _argc = argc;
        _argv = argv;
    }

    ~Service() override {
    }
// IService interface
protected:

    /**
     * @brief createApplication default implementation create a Application object.
     */
    void createApplication() override {
        static_assert (std::is_base_of<Application, QCoreApplication>::value,
                "the Application type must be QCoreApplication");
        setCore(new Application(_argc, _argv));
    }

private:
    int _argc = 0;
    char **_argv = 0;
};

}


#endif // SERVICE_H
