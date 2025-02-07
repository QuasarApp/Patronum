/*
 * Copyright (C) 2018-2025 QuasarApp.
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

    qInfo() << "Shutdown application with " << sig << " signal.";
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
        qCritical() << "socket is closed!";

        return false;
    }

    return _socket->send(_parser->createPackage(Command::FeatureResponce, result));
}

bool ServicePrivate::sendCloseConnection() {
    if (!_socket->isValid()) {
        qCritical() << "socket is closed!";

        return false;
    }

    return _socket->send(_parser->createPackage(Command::CloseConnection));
}

bool ServicePrivate::install(const QString &user) {

    if (!_installer) {
        qCritical() << errorToString(UnsupportedPlatform);
        return false;
    }

    if (!_installer->install(getServiceLauncher(), user)) {
        return false;
    }

    qInfo() << "The service installed successful";
    return true;
}

bool ServicePrivate::uninstall() {
    if (!_installer) {
        qCritical() << errorToString(UnsupportedPlatform);

        return false;
    }

    if (!_installer->uninstall()) {
        return false;
    }

    qInfo() << "The service uninstalled successful";
    return true;
}

bool ServicePrivate::start() {

    if (!_socket->listen()) {

        qCritical() << "Fail to create a terminal socket!";
        return false;
    };

    return _service->onStart();
}

bool ServicePrivate::startDeamon() {

    if (_socket->isRunning()) {
        qCritical() << "Failed to start a service because an another service alredy started";
        return false;
    }

    QProcess proc;
    proc.setProgram(QuasarAppUtils::Params::getCurrentExecutable());

    proc.setArguments({"start"});
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setProcessChannelMode(QProcess::SeparateChannels);

    qint64 pid;
    if (!proc.startDetached(&pid)) {
        qCritical() << "fail to start detached process: " + proc.errorString();
        return false;
    }

    QFile pidFile(PCommon::instance()->getPidfile());
    if (!pidFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    pidFile.write(QByteArray::number(pid));
    pidFile.close();

    qInfo() << "The service started successful";
    return true;
}

bool ServicePrivate::handleStandartCmd(QHash<QString, Feature> *cmds) {

    if (!cmds)
        return false;

    if (!_service)
        return false;

    if (cmds->contains("stop")) {
        _service->onStop();
        cmds->remove("stop");

    } else if (cmds->contains("pause")) {
        _service->onPause();
        cmds->remove("pause");

    } else if (cmds->contains("resume")) {
        _service->onResume();
        cmds->remove("resume");

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

    for (const auto &pkg: std::as_const(packages)) {
        if (!pkg.isValid()) {

            qWarning() << "Invalid Package received. ";
            return;
        }

        if (!_service) {

            qCritical() << "System error, service is not inited!";
            return;;
        }

        if (!_socket->isValid()) {

            qCritical() << "scoket is closed!";
            return;
        }

        switch (pkg.cmd()) {

        case Command::FeaturesRequest: {

            if (!_socket->send(_parser->createPackage(Command::Features,
                                                      _service->supportedFeatures()))) {
                qCritical() << "Fail to send supported features!";
            }

            break;

        }

        case Command::Feature: {

            QDataStream stream(pkg.data());

            QHash<QString, Feature> feature;
            stream >> feature;
            handleStandartCmd(&feature);

            if (feature.size())
                _service->handleReceiveData(feature);

            break;

        }

        default: {
            qCritical() << "Wrong command!";
            break;
        }
        }

    }

    sendCloseConnection();

}

}
