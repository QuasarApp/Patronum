/*
 * Copyright (C) 2018-2021 QuasarApp.
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
#include "parser.h"

namespace Patronum {

Patronum::ServicePrivate::ServicePrivate(const QString &name, IService *service, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name, this);

    _service = service;

    _parser = new Parser();
    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ServicePrivate::handleReceve);

}

ServicePrivate::~ServicePrivate() {
    delete _parser;
}

bool ServicePrivate::sendCmdResult(const QVariantMap &result) {

    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Error);
        return false;
    }

    return _socket->send(_parser->createPackage(Command::FeatureResponce, result));
}

bool ServicePrivate::sendCloseConnection() {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Error);
        return false;
    }

    return _socket->send(_parser->createPackage(Command::CloseConnection));
}

void ServicePrivate::listen() const {
    if (!_socket->listen()) {
        QuasarAppUtils::Params::log("Fail to create a terminal socket!");
        QCoreApplication::exit(1);
    };
}

bool ServicePrivate::handleStandartCmd(QSet<Feature> *cmds) {

    if (!cmds)
        return false;

    if (!_service)
        return false;

    if (cmds->contains(Feature{"stop"})) {
        _service->onStop();
        cmds->remove(Feature{"stop"});

    } else if (cmds->contains(Feature{"pause"})) {
        _service->onPause();
        cmds->remove(Feature{"pause"});

    } else if (cmds->contains(Feature{"resume"})) {
        _service->onResume();
        cmds->remove(Feature{"resume"});

    }

    return true;
}

void ServicePrivate::handleReceve(QByteArray data) {

    QList<Package> packages;
    if (!_parser->parse(data, packages)) {
        return;
    }

    for (const auto &pkg: qAsConst(packages)) {
        if (!pkg.isValid()) {
            QuasarAppUtils::Params::log("receive package is not valid!",
                                        QuasarAppUtils::Warning);
            return;
        }

        if (!_service) {
            QuasarAppUtils::Params::log("System error, service is not inited!",
                                        QuasarAppUtils::Error);
            return;;
        }

        if (!_socket->isValid()) {
            QuasarAppUtils::Params::log("scoket is closed!",
                                        QuasarAppUtils::Error);
            return;
        }

        switch (pkg.cmd()) {

        case Command::FeaturesRequest: {

            if (!_socket->send(_parser->createPackage(Command::Features,
                                                      _service->supportedFeatures()))) {
                QuasarAppUtils::Params::log("Fail to send ",
                                            QuasarAppUtils::Error);
            }

            break;

        }

        case Command::Feature: {

            QDataStream stream(pkg.data());

            QSet<Feature> feature;
            stream >> feature;
            handleStandartCmd(&feature);

            if (feature.size())
                _service->handleReceiveData(feature);

            break;

        }

        default: {
            QuasarAppUtils::Params::log("Wrong command!",
                                        QuasarAppUtils::Error);
            break;
        }
        }

    }

    sendCloseConnection();

}

}
