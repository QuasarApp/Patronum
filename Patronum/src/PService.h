/*
 * Copyright (C) 2018-2020 QuasarApp.
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
 * ###How to use :
 * - just inherit from the Service class and override the methods you need.
 */
class PATRONUM_LIBRARYSHARED_EXPORT Service : public ServiceBase
{
public:
    /**
     * @brief Service
     * @param argc - count params
     * @param argv - test of arguments
     * @param name - name of your service
     */
    Service(int argc, char *argv[], const QString &name)
        : ServiceBase(argc, argv, name) {

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
        static_assert (std::is_base_of<Application, QCoreApplication>::value, "the Application type must be QCoreApplication");
        _core = new Application(_argc, _argv);
    }

private:
    ServicePrivate *d_ptr = nullptr;
    int _argc = 0;
    char **_argv = 0;
};

}


#endif // SERVICE_H
