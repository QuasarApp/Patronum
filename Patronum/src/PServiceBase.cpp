/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "PServiceBase.h"
#include <QCoreApplication>
#include <QFile>
#include <QLibraryInfo>
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>
#include <pcommon.h>
#include "PController.h"
#include "serviceprivate.h"

namespace Patronum {

ServiceBase::ServiceBase(int argc, char *argv[]) {
    QuasarAppUtils::Params::parseParams(argc, argv);
    d_ptr = new ServicePrivate(this);

}

ServiceBase::~ServiceBase() {
    delete d_ptr;

    QFile pidFile(PCommon::instance()->getPidfile());
    if (pidFile.exists()) {
        pidFile.remove();
    }

    if (_core) {
        delete _core;
    }

    if (_controller) {
        delete _controller;
    }
}

void ServiceBase::handleReceiveData(const QSet<Feature> &data) {
    auto list = supportedFeatures();

    QString commandList;

    for (const auto& i: data) {
        if (list.contains(i)) {
            if (!handleReceive(i)) {
                sendResuylt(QString("The process of a command %0 with argumets: %1 is failed")
                            .arg(i.cmd(), i.arg()));
            }
        } else {
            commandList += i.toString() + " ";
        }
    }

    if (commandList.size()) {
        QString stringList;

        for (const auto&i : list) {
            stringList += i.toString() + " ";
        }

        QVariantMap result;

        result["Error"] = "Wrong command! The commands : " + commandList  + " is not supported";
        result["Available commands"] = stringList;

        sendResuylt(result);
    }
}

QSet<Feature> ServiceBase::supportedFeatures() {
    return {};
}

bool ServiceBase::sendResuylt(const QVariantMap &result) {
    return d_ptr->sendCmdResult(result);
}

bool ServiceBase::sendResuylt(const QString &result) {
    return d_ptr->sendCmdResult({{"Result", result}});
}

void ServiceBase::onStop() {
    sendResuylt("Success! Use default stop function");

    QTimer::singleShot(1000, nullptr, [](){
        QCoreApplication::quit();
    });
}

void ServiceBase::onResume() {
    sendResuylt("This function not supported");
}

void ServiceBase::onPause() {
    sendResuylt("This function not supported");
}

Controller *ServiceBase::controller() {
    if (_controller)
        return _controller;

    _controller = new Controller();

    return _controller;
}

void ServiceBase::printDefaultHelp() {
    auto serviceHelp = controller()->help();
    serviceHelp.unite(QuasarAppUtils::Params::getHelp());

    serviceHelp.unite({{QObject::tr("Options that available befor install"),{
                            {"start / s",       QObject::tr("Start a service in console")},
                            {"daemon / d",      QObject::tr("Start a service as a daemon")},
                            {"install / i / -install username",     QObject::tr("Install a service. Add user name if you want to run service from custom user. Example: -install username")},
                            {"unistall / u",    QObject::tr("unistall a service")},
                        }}});

    const auto features = supportedFeatures();
    QuasarAppUtils::Help::Options optionsList;
    for (const auto& cmd : features) {
        optionsList.insert(cmd.cmd(), cmd.description());
    }

    serviceHelp.unite({{"Available commands of the service:", optionsList}});

    QuasarAppUtils::Help::print(serviceHelp);

}

QCoreApplication *ServiceBase::core() {
    return _core;
}

void ServiceBase::setCore(QCoreApplication *core) {
    if (_core)
        delete _core;

    _core = core;
}

int ServiceBase::exec() {
    if (!core()) {
        createApplication();
    }


    bool printHelp = !QuasarAppUtils::Params::size() ||
            QuasarAppUtils::Params::isEndable("h") ||
            QuasarAppUtils::Params::isEndable("help");

    if (printHelp) {
        printDefaultHelp();
        return 0;
    }

    bool fStart = QuasarAppUtils::Params::isEndable("start") || QuasarAppUtils::Params::isEndable("s");
    bool fDaemon = QuasarAppUtils::Params::isEndable("daemon") || QuasarAppUtils::Params::isEndable("d");

    if (QuasarAppUtils::Params::isEndable("install") || QuasarAppUtils::Params::isEndable("i")) {
        if (!d_ptr->install(QuasarAppUtils::Params::getArg("install", "root")))
            return Patronum::PatronumError::UnsupportedPlatform;
        return 0;
    }

    if (QuasarAppUtils::Params::isEndable("uninstall") || QuasarAppUtils::Params::isEndable("u")) {
        if (!d_ptr->uninstall())
            return Patronum::PatronumError::UnsupportedPlatform;
        return 0;
    }

    if (fStart || fDaemon) {

        if (fDaemon) {
            if (!d_ptr->startDeamon())
                return Patronum::PatronumError::UnsupportedPlatform;
            return 0;
        }

        QTimer::singleShot(0, nullptr, [this]() {
            d_ptr->start();
        });
    } else {
        QTimer::singleShot(0, nullptr, [this]() {
            if (!controller()->send()) {
                core()->exit(static_cast<int>(PatronumError::ServiceUnavailable));
            }
        });
    }

    return core()->exec();
}

}

