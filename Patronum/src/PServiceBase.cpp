/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "PServiceBase.h"
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QTimer>
#include "PController.h"
#include "serviceprivate.h"

namespace Patronum {

ServiceBase::ServiceBase(int argc, char *argv[], const QString &name) {
    QuasarAppUtils::Params::parseParams(argc, argv);
    d_ptr = new ServicePrivate(name, this);
    _serviceName = name;

}

ServiceBase::~ServiceBase() {
    delete d_ptr;
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
    QCoreApplication::quit();

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

    _controller = new Controller(_serviceName,
                                 QuasarAppUtils::Params::getCurrentExecutable());

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

int ServiceBase::exec() {
    if (!_core) {
        createApplication();
    }

    if (!QuasarAppUtils::Params::size()) {
        printDefaultHelp();
        return 0;
    }

    bool fStart = QuasarAppUtils::Params::isEndable("start") || QuasarAppUtils::Params::isEndable("s");
    bool fDaemon = QuasarAppUtils::Params::isEndable("daemon") || QuasarAppUtils::Params::isEndable("d");

    if (fStart || fDaemon) {

        if (fDaemon) {
            return controller()->startDetached();
        }

        QTimer::singleShot(0, nullptr, [this](){
            onStart();
            d_ptr->listen();

        });
    } else {
        QTimer::singleShot(0, nullptr, [this](){
            if (!controller()->send()) {
                _core->exit(static_cast<int>(ControllerError::ServiceUnavailable));
            }
        });
    }

    return _core->exec();
}

}

