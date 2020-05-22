/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "PController.h"
#include "controllerprivate.h"
#include <QDateTime>
#include <QFileInfo>
#include <QVariantMap>
#include <quasarapp.h>
#include <QCoreApplication>

namespace Patronum {

Controller::Controller(const QString &name, const QString& servicePath) {
    d_ptr = new ControllerPrivate(name, servicePath, this);
}

Controller::~Controller() {
    delete d_ptr;
}

bool Controller::send(int argc, char **argv) {
    if (!QuasarAppUtils::Params::size() && !QuasarAppUtils::Params::parseParams(argc, argv)) {
        return false;
    }

    return send();
}

bool Controller::send() {

    if (QuasarAppUtils::Params::isEndable("start")) {
        return !d_ptr->start();
    }

    if (QuasarAppUtils::Params::isEndable("install")) {
        return d_ptr->install();
    }

    if (QuasarAppUtils::Params::isEndable("uninstall")) {
        return d_ptr->uninstall();
    }

    bool printHelp = !QuasarAppUtils::Params::customParamasSize() ||
            QuasarAppUtils::Params::isEndable("h") ||
            QuasarAppUtils::Params::isEndable("help");

    if (printHelp) {
        printDefaultHelp();
    }

    if (!d_ptr->connectToHost()) {
        return false;
    }

    if (printHelp) {
        if (!d_ptr->sendFeaturesRequest()) {
            return false;
        }
        return true;
    }

    if (QuasarAppUtils::Params::isEndable("stop")) {
        return d_ptr->stop();
    }

    if (QuasarAppUtils::Params::isEndable("resume")) {
        return d_ptr->resume();
    }

    if (QuasarAppUtils::Params::isEndable("pause")) {
        return d_ptr->pause();
    }

    QList<Feature> sendData = {};
    auto userParams = QuasarAppUtils::Params::getUserParamsMap();
    for (auto val = userParams.begin(); val != userParams.end(); ++val) {
        if (val.key() == "verbose" || val.key() == "fileLog" || val.key() == "exec") {
            continue;
        }

        sendData += {val.key(), val.value()};
    }

    return d_ptr->sendCmd(sendData);
}

int Controller::startDetached() const {
    return d_ptr->start();
}

bool Controller::waitForResponce(int msec) {

    return d_ptr->waitForResponce(msec);
}

void Controller::handleError(ControllerError error) {
    QuasarAppUtils::Params::log(errorToString(error),
                                QuasarAppUtils::Error);
}

void Controller::handleFeatures(const QList<Feature> &features) {

    QuasarAppUtils::Help::Options options;

    for (const auto& feature: features ) {
        QString cmd, description;
        if (feature.arg().isNull()) {
            cmd = feature.cmd();
        } else {
            cmd = QString("-%0 value").arg(feature.cmd());
        }

        description = feature.description();

        if (!feature.example().isEmpty()) {
            description += ". Example : " + feature.example();
        }

        options.insert(cmd, description);
    }

    QuasarAppUtils::Help::print(options);
    QCoreApplication::exit(0);
}

void Controller::handleResponce(const QVariantMap &responce) {
    QuasarAppUtils::Help::Options options;

    for(auto iter = responce.begin(); iter != responce.end(); ++iter) {
        options.insert(iter.key(), iter.value().toString());
    }

    QuasarAppUtils::Help::print(options);
    QCoreApplication::exit(0);
}

QList<Feature> Controller::features() {
    return d_ptr->features();
}

void Controller::printDefaultHelp() const {

    auto quasarappHelp = QuasarAppUtils::Params::getparamsHelp();

    QuasarAppUtils::Help::Charters help{{"General options of this controller",{
                {"start",    QObject::tr("Start a service")},
                {"stop",     QObject::tr("Stop a service")},
                {"pause",    QObject::tr("Pause a service")},
                {"resume",   QObject::tr("Resume a service")},
                {"install",  QObject::tr("Install a service")},
                {"uninstall", QObject::tr("Uninstall a service")}
            }}};

    QuasarAppUtils::Help::print(quasarappHelp.unite(help));
}

}
