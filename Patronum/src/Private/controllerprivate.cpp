#include "controllerprivate.h"
#include "IPController.h"
#include "localsocket.h"
#include <QCoreApplication>
#include <QDateTime>
#include <quasarapp.h>
#include "package.h"

namespace Patronum {

ControllerPrivate::ControllerPrivate(const QString &name, IController *controller, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name, this);

    if (!_socket->connectToTarget()) {
        QuasarAppUtils::Params::log("Connect to service fail !");
    };

    _controller = controller;

    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ControllerPrivate::handleReceve);

}

bool ControllerPrivate::sendFeaturesRequest() {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Error);
        return false;
    }

    QByteArray responce;
    QDataStream stream(&responce, QIODevice::WriteOnly);

    stream << static_cast<quint8>(Command::FeaturesRequest);

    return _socket->send(responce);
}

bool ControllerPrivate::sendCmd(const QList<Feature> &result) {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Error);
        return false;
    }

    QByteArray request;
    QDataStream stream(&request, QIODevice::WriteOnly);

    stream << static_cast<quint8>(Command::Feature);
    stream << result;

    if (_socket->send(request)) {
        _responce = false;
        return true;
    }

    return false;
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
    return _responce;
}

void ControllerPrivate::handleReceve(QByteArray data) {

    const Package package = Package::parsePackage(data);

    if (!package.isValid()) {
        return;
    }

    switch (package.cmd()) {

    case Command::Features: {

        if (!_controller) {
            QuasarAppUtils::Params::log("System error, controller is not inited!",
                                        QuasarAppUtils::Error);
            break;
        }

        QDataStream stream(package.data());

        QList<Feature> features;
        stream >> features;
        _features = features;

        _responce = true;

        _controller->handleFeatures(features);

        break;

    }

    case Command::FeatureResponce: {
        if (!_controller) {
            QuasarAppUtils::Params::log("System error, controller is not inited!",
                                        QuasarAppUtils::Error);
            break;
        }

        QDataStream stream(package.data());

        _responce = true;

        QVariantMap responce;
        stream >> responce;
        _controller->handleResponce(responce);

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
