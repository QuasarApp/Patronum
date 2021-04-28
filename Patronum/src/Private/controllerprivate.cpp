/*
 * Copyright (C) 2018-2021 QuasarApp.
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
#include <quasarapp.h>
#include "package.h"
#include "installersystemd.h"
#include "parser.h"

namespace Patronum {

ControllerPrivate::ControllerPrivate(const QString &name, const QString &servicePath,
                                     IController *controller, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name, this);
    _serviceExe = servicePath;
    _controller = controller;
    _parser = new Parser();

#ifdef Q_OS_LINUX
    _installer = new InstallerSystemD(name);
#endif

    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ControllerPrivate::handleReceve);

}

ControllerPrivate::~ControllerPrivate() {
    if (_installer) {
        delete _installer;
    }
    delete _parser;
}

bool ControllerPrivate::sendFeaturesRequest() {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::ServiceUnavailable);
        return false;
    }

    return _socket->send(_parser->createPackage(Command::FeaturesRequest));
}

bool ControllerPrivate::sendCmd(const QSet<Feature> &result) {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::ServiceUnavailable);

        return false;
    }

    if (_socket->send(_parser->createPackage(Command::Feature, result))) {
        _responce = false;
        return true;
    }

    return false;
}

int ControllerPrivate::start() const {

    QProcess proc;
    proc.setProgram(getExecutable());

    proc.setArguments({"d"});
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setProcessChannelMode(QProcess::SeparateChannels);

    if (!proc.startDetached()) {
        QuasarAppUtils::Params::log("fail to start detached process: " + proc.errorString(),
                                    QuasarAppUtils::Error);
        return  static_cast<int>(ControllerError::SystemError);
    }

    _controller->handleResponce({{"Result", "start service successful"}});

    return 0;
}

bool ControllerPrivate::stop() {
    return sendCmd({Feature("stop")});
}

bool ControllerPrivate::install() const {
    if (!_installer) {
        QuasarAppUtils::Params::log("Unsupported platform",
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!_installer->install(getExecutable())) {
        return false;
    }

    _controller->handleResponce({{"Result", "Install service successful"}});

    return true;
}

bool ControllerPrivate::uninstall() const {
    if (!_installer) {
        QuasarAppUtils::Params::log("Unsupported platform",
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!_installer->uninstall()) {
        return false;
    }

    _controller->handleResponce({{"Result", "Uninstall service successful"}});

    return true;
}

bool ControllerPrivate::pause() {
    return sendCmd({Feature("pause")});
}

bool ControllerPrivate::resume() {
    return sendCmd({Feature("resume")});
}

bool Patronum::ControllerPrivate::waitForResponce(int msec) {
    if (!dynamic_cast<QCoreApplication*>(QCoreApplication::instance())) {
        QuasarAppUtils::Params::log("Before run the waitForResponce method you need run a exec method of your QApplication class.",
                                    QuasarAppUtils::Warning);

        return false;
    }

    qint64 waitFor = QDateTime::currentMSecsSinceEpoch() + msec;

    while (!_responce && QDateTime::currentMSecsSinceEpoch() < waitFor) {
        QCoreApplication::processEvents();
    }
    QCoreApplication::processEvents();

    return _responce;
}

QList<Feature> ControllerPrivate::features() const {
    return _features;
}

bool ControllerPrivate::isConnected() const {
    return _socket->isValid();
}

bool ControllerPrivate::connectToHost() const {
    if (isConnected()) {
        return true;
    }

    if (!_socket->connectToTarget()) {
        QuasarAppUtils::Params::log("Connect to service fail !",
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::ServiceUnavailable);

        return false;

    };

    return true;
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

            _controller->handleError(ControllerError::InvalidPackage);

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
            _responce = true;
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
            _controller->handleError(ControllerError::WrongCommand);

            break;
        }
    }

    }

}

QString ControllerPrivate::getExecutable() const {
    if (QFile::exists(_serviceExe)) {
        return _serviceExe;
    }

    if (!_installer) {
        return "";
    }

    return _installer->getExecutable();
}
}
