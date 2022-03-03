/*
 * Copyright (C) 2018-2022 QuasarApp.
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
#include <QTimer>

namespace Patronum {

Controller::Controller() {
    d_ptr = new ControllerPrivate(this);
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

    bool printHelp = !QuasarAppUtils::Params::size() ||
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

    QSet<Feature> sendData = {};
    auto userParams = QuasarAppUtils::Params::getUserParamsMap();
    for (auto val = userParams.begin(); val != userParams.end(); ++val) {

        bool fIgnore = val.key() == "verbose" || val.key() == "fileLog";

        if (fIgnore) {
            continue;
        }

        sendData.insert(Feature{val.key(), val.value()});
    }

    if (!d_ptr->sendCmd(sendData)) {
        return false;
    }

    QTimer::singleShot(1000, nullptr, []() {
        QuasarAppUtils::Params::log(errorToString(PatronumError::TimeOutError), QuasarAppUtils::Error);
        QCoreApplication::exit(static_cast<int>(PatronumError::TimeOutError));
    });

    return true;
}

bool Controller::sendStop() {
    if (!d_ptr)
        return false;

    d_ptr->setEcho(false);

    if (!d_ptr->connectToHost(false)) {
        return false;
    }

    _disableFinished = true;

    return d_ptr->stop();
}

QuasarAppUtils::Help::Section Controller::help() const {
    QuasarAppUtils::Help::Section help {
        {QObject::tr("Options that available after start"), {
                {"stop",            QObject::tr("Stop a service")},
                {"pause",           QObject::tr("Pause a service")},
                {"resume",          QObject::tr("Resume a service")}

            }
        }
    };

    return help;
}

void Controller::handleError(PatronumError error) {
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
}

void Controller::handleResponce(const QVariantMap &responce) {
    QuasarAppUtils::Help::Options options;

    for(auto iter = responce.begin(); iter != responce.end(); ++iter) {
        options.insert(iter.key(), iter.value().toString());
    }

    QuasarAppUtils::Help::print(options);
}

void Controller::finished() {
    if (!_disableFinished)
        QCoreApplication::exit(0);
}

QList<Feature> Controller::features() {
    return d_ptr->features();
}

void Controller::printDefaultHelp() const {

    auto quasarappHelp = QuasarAppUtils::Params::getHelp();
    QuasarAppUtils::Help::print(quasarappHelp.unite(help()));
}

}
