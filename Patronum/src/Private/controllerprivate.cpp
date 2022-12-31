/*
 * Copyright (C) 2018-2023 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "controllerprivate.h"
#include "IPController.h"
#include "localsocket.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <quasarapp.h>
#include "package.h"
#include "parser.h"
#include "pcommon.h"

namespace Patronum {

ControllerPrivate::ControllerPrivate(IController *controller, QObject *parent):
    QObject(parent) {
    QString name = PCommon::instance()->getServiceName();
    _socket = new LocalSocket(name, this);
    _controller = controller;
    _parser = new Parser();

    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ControllerPrivate::handleReceve);

}

ControllerPrivate::~ControllerPrivate() {
    delete _parser;
    delete _socket;
}

bool ControllerPrivate::sendFeaturesRequest() {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Debug);
        _controller->handleError(PatronumError::ServiceUnavailable);
        return false;
    }

    return _socket->send(_parser->createPackage(Command::FeaturesRequest));
}

bool ControllerPrivate::sendCmd(const QSet<Feature> &result) {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Debug);
        _controller->handleError(PatronumError::ServiceUnavailable);

        return false;
    }

    if (_socket->send(_parser->createPackage(Command::Feature, result))) {
        return true;
    }

    return false;
}

bool ControllerPrivate::stop() {
    return sendCmd({Feature("stop")});
}

bool ControllerPrivate::pause() {
    return sendCmd({Feature("pause")});
}

bool ControllerPrivate::resume() {
    return sendCmd({Feature("resume")});
}

QList<Feature> ControllerPrivate::features() const {
    return _features;
}

bool ControllerPrivate::isConnected() const {
    return _socket->isValid();
}

bool ControllerPrivate::connectToHost(bool echo) const {
    if (isConnected()) {
        return true;
    }

    if (!_socket->connectToTarget()) {

        if (echo) {
            QuasarAppUtils::Params::log("Connect to service fail !",
                                        QuasarAppUtils::Debug);
            _controller->handleError(PatronumError::ServiceUnavailable);
        }

        return false;

    };

    return true;
}

void ControllerPrivate::setEcho(bool echo) {
    if (_socket)
        _socket->setEcho(echo);
}

void ControllerPrivate::handleReceve(QByteArray data) {

    if (!_controller) {
        QuasarAppUtils::Params::log("System error, controller is not inited!",
                                    QuasarAppUtils::Error);
        return;
    }

    QList<Package> packages;
    if (!_parser->parse(data, packages)) {
        return;
    }

    for (const auto& pkg: qAsConst(packages)) {
        if (!pkg.isValid()) {

            QuasarAppUtils::Params::log("Received invalid package!",
                                        QuasarAppUtils::Debug);

            _controller->handleError(PatronumError::InvalidPackage);

            continue;;
        }

        switch (pkg.cmd()) {

        case Command::Features: {

            QDataStream stream(pkg.data());

            QList<Feature> features;
            stream >> features;
            _features = features;

            _controller->handleFeatures(features);

            break;

        }

        case Command::CloseConnection: {
            _controller->finished();
            break;
        }

        case Command::FeatureResponce: {

            QDataStream stream(pkg.data());


            QVariantMap responce;
            stream >> responce;
            _controller->handleResponce(responce);

            break;

        }
        default: {
            QuasarAppUtils::Params::log("Wrong command!",
                                        QuasarAppUtils::Debug);
            _controller->handleError(PatronumError::WrongCommand);

            break;
        }
    }

    }

}
}
