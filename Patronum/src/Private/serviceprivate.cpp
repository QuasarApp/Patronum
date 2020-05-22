/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "serviceprivate.h"

#include "IPService.h"
#include "localsocket.h"
#include "package.h"
#include <QCoreApplication>
#include <QTimer>
#include <quasarapp.h>

namespace Patronum {

Patronum::ServicePrivate::ServicePrivate(const QString &name, IService *service, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name, this);

    _service = service;

    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ServicePrivate::handleReceve);

}

bool ServicePrivate::sendCmdResult(const QVariantMap &result) {

    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Error);
        return false;
    }

    return _socket->send(Package::createPackage(Command::FeatureResponce, result));
}

void ServicePrivate::listen() const {
    if (!_socket->listen()) {
        QuasarAppUtils::Params::log("Fail to create a terminal socket!");
        QCoreApplication::exit(1);
    };
}

bool ServicePrivate::hendleStandartCmd(QList<Feature> *cmds) {

    if (!cmds)
        return false;

    if (!_service)
        return false;

    for (int i = 0; i < cmds->size(); ++i) {
        if (cmds->value(i).cmd() == "stop") {
            _service->onStop();
            cmds->removeAt(i);
            i--;
        } else if (cmds->value(i).cmd() == "pause") {
            _service->onPause();
            cmds->removeAt(i);
            i--;
        } else if (cmds->value(i).cmd() == "resume") {
            _service->onResume();
            cmds->removeAt(i);
            i--;
        }

    }

    return true;
}

void ServicePrivate::handleReceve(QByteArray data) {

    const Package package = Package::parsePackage(data);

    if (!package.isValid()) {
        QuasarAppUtils::Params::log("receive package is not valid!",
                                    QuasarAppUtils::Warning);
        return;
    }

    switch (package.cmd()) {

    case Command::FeaturesRequest: {

        if (!_service) {
            QuasarAppUtils::Params::log("System error, service is not inited!",
                                        QuasarAppUtils::Error);
            break;
        }

        if (!_socket->isValid()) {
            QuasarAppUtils::Params::log("scoket is closed!",
                                        QuasarAppUtils::Error);
            break;
        }

        QList<Feature> features = _service->supportedFeatures();
        QByteArray sendData;
        QDataStream stream(&sendData, QIODevice::WriteOnly);

        stream << static_cast<quint8>(Command::Features);
        stream << features;

        if (!_socket->send(sendData)) {
            QuasarAppUtils::Params::log("scoket is closed!",
                                        QuasarAppUtils::Error);
        }

        break;

    }

    case Command::Feature: {
        if (!_service) {
            QuasarAppUtils::Params::log("System error, service is not inited!",
                                        QuasarAppUtils::Error);
            break;
        }

        QDataStream stream(package.data());

        QList<Feature> feature;
        stream >> feature;
        hendleStandartCmd(&feature);

        if (feature.size())
            _service->handleReceive(feature);

        break;

    }

    default: {
        QuasarAppUtils::Params::log("Wrong command!",
                                    QuasarAppUtils::Error);
        break;
    }

    }

}

}
