/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "pcommon.h"
#include "serviceprivate.h"
#include "installersystemd.h"

#include "IPService.h"
#include "localsocket.h"
#include "package.h"
#include <QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <quasarapp.h>
#include "parser.h"
#include <csignal>

void handleTermSignals(int sig) {
    QuasarAppUtils::Params::log("Shutdown application CTRL+C.", QuasarAppUtils::Info);
    QCoreApplication::exit(0);
}

namespace Patronum {

Patronum::ServicePrivate::ServicePrivate(IService *service, QObject *parent):
    QObject(parent) {
    QString name = PCommon::instance()->getServiceName();

    _socket = new LocalSocket(name, this);

    _service = service;

#ifdef Q_OS_LINUX
    _installer = new InstallerSystemD();
#endif

    _parser = new Parser();
    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ServicePrivate::handleReceve);

    signal(SIGINT, &handleTermSignals);
    signal(SIGTERM, &handleTermSignals);

}

ServicePrivate::~ServicePrivate() {
    delete _parser;
    delete _socket;

    if (_installer) {
        delete _installer;
    }
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

bool ServicePrivate::install(const QString &user) {

    if (!_installer) {
        QuasarAppUtils::Params::log(errorToString(UnsupportedPlatform),
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!_installer->install(getServiceLauncher(), user)) {
        return false;
    }

    QuasarAppUtils::Params::log("The service installed successful", QuasarAppUtils::Info);
    return true;
}

bool ServicePrivate::uninstall() {
    if (!_installer) {
        QuasarAppUtils::Params::log(errorToString(UnsupportedPlatform),
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!_installer->uninstall()) {
        return false;
    }

    QuasarAppUtils::Params::log("The service uninstalled successful", QuasarAppUtils::Info);
    return true;
}

bool ServicePrivate::start() {

    if (!_socket->listen()) {
        QuasarAppUtils::Params::log("Fail to create a terminal socket!",
                                    QuasarAppUtils::Error);
        return false;
    };

    return _service->onStart();
}

bool ServicePrivate::startDeamon() {

    if (_socket->isRunning()) {
        QuasarAppUtils::Params::log("Failed to start a service because an another service alredy started",
                                    QuasarAppUtils::Error);
        return false;
    }

    QProcess proc;
    proc.setProgram(QuasarAppUtils::Params::getCurrentExecutable());

    proc.setArguments({"start"});
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setProcessChannelMode(QProcess::SeparateChannels);

    qint64 pid;
    if (!proc.startDetached(&pid)) {
        QuasarAppUtils::Params::log("fail to start detached process: " + proc.errorString(),
                                    QuasarAppUtils::Error);
        return false;
    }

    QFile pidFile(PCommon::instance()->getPidfile());
    if (!pidFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    pidFile.write(QByteArray::number(pid));
    pidFile.close();

    QuasarAppUtils::Params::log("The service started successful", QuasarAppUtils::Info);

    return true;
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

QString ServicePrivate::getServiceLauncher() const {
    const QByteArray P_RUN_FILE = qgetenv("P_RUN_FILE");
    if (P_RUN_FILE.size()) {
        return P_RUN_FILE;
    }

    const QByteArray CQT_RUN_FILE = qgetenv("CQT_RUN_FILE");
    if (CQT_RUN_FILE.size()) {
        return CQT_RUN_FILE;
    }

    return QuasarAppUtils::Params::getCurrentExecutable();
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
