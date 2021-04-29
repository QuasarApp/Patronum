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
        QStringList stringList;

        for (const auto&i : list) {
            stringList += i.toString();
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

bool ServiceBase::sendCloseeConnetion() {
    return d_ptr->sendCloseConnection();
}

void ServiceBase::onStop() {
    sendResuylt("Success! Use default stop function");

    QTimer::singleShot(100, nullptr, [](){
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

    _controller = new Controller(QuasarAppUtils::Params::getCurrentExecutable());

    return _controller;
}

void ServiceBase::printDefaultHelp() {
    auto serviceHelp = controller()->help();
    serviceHelp.unite(QuasarAppUtils::Params::getParamsHelp());

    serviceHelp.unite({{QObject::tr("Options that available befor install"),{
                            {"start / s",       QObject::tr("Start a service in console")},
                            {"daemon / d",      QObject::tr("Start a service as a daemon")},
                            {"install / i",     QObject::tr("Install a service")},
                        }}});

    const auto features = supportedFeatures();
    QuasarAppUtils::Help::Options optionsList;
    for (const auto& cmd : features) {
        optionsList.insert(cmd.cmd(), cmd.description());
    }

    serviceHelp.unite({{"Available commands of the service:", optionsList}});

    QuasarAppUtils::Help::print(serviceHelp);

}

void ServiceBase::startThisService() {
    onStart();
    if (d_ptr->listen()) {
        QFile pidFile(PCommon::instance()->getPidfile());
        if (pidFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            pidFile.write(QByteArray::number(QCoreApplication::applicationPid()));
            pidFile.close();
        }
    }
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

    if (fStart || fDaemon) {

        if (fDaemon) {
            return controller()->startDetached();
        }

        QTimer::singleShot(0, nullptr, [this]() {
            startThisService();
        });
    } else {
        QTimer::singleShot(0, nullptr, [this]() {
            if (!controller()->send()) {
                core()->exit(static_cast<int>(ControllerError::ServiceUnavailable));
            }
        });
    }

    return core()->exec();
}

}

