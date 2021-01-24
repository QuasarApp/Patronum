/*
 * Copyright (C) 2018-2020 QuasarApp.
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

void ServiceBase::handleReceive(const QList<Feature> &data) {

    auto list = supportedFeatures();

    QStringList stringList;

    for (const auto&i : list) {
        stringList += i.toString();
    }

    QVariantMap result;

    QString commandList;
    for (const auto&i : data ) {
        commandList += i.toString() + " ";
    }

    result["Error"] = "Wrong command! The commands : " + commandList  + " is not supported";
    result["Available commands"] = stringList;

    sendResuylt(result);

}

QList<Feature> ServiceBase::supportedFeatures() {
    QList<Feature> result;
    return result;
}

bool ServiceBase::sendResuylt(const QVariantMap &result) {
    return d_ptr->sendCmdResult(result);
}

bool ServiceBase::sendResuylt(const QString &result) {
    return d_ptr->sendCmdResult({{"Result", result}});
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

int ServiceBase::exec() {
    if (!_core) {
        createApplication();
    }

    bool fExec = QuasarAppUtils::Params::isEndable("exec") || QuasarAppUtils::Params::isDebugBuild();

    if (!(QuasarAppUtils::Params::customParamasSize() || fExec)) {
        return controller()->startDetached();
    }

    if (fExec) {
        QTimer::singleShot(0, [this](){
            onStart();
            d_ptr->listen();

        });
    } else {
        QTimer::singleShot(0, [this](){
            if (!controller()->send()) {
                _core->exit(static_cast<int>(ControllerError::ServiceUnavailable));
            }
        });
    }

    return _core->exec();
}

}

