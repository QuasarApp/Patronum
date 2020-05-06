#include "serviceprivate.h"

#include "IPService.h"
#include "localsocket.h"
#include "package.h"
#include <QCoreApplication>
#include <quasarapp.h>

namespace Patronum {

Patronum::ServicePrivate::ServicePrivate(const QString &name, IService *service, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name);

    if (!_socket->listen()) {
        QuasarAppUtils::Params::log("Fail to create a terminal socket!");
        QCoreApplication::exit(1);
    };

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

    QByteArray responce;
    QDataStream stream(&responce, QIODevice::WriteOnly);

    stream << Command::FeatureResponce << result;

    return _socket->send(responce);
}

void ServicePrivate::handleReceve(QByteArray data) {

    if (data.size() < 2) {
        return;
    }

    const Package *package = reinterpret_cast<const Package *>( data.data());

    switch (package->cmd) {

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

        stream << Command::Features << features;
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

        QDataStream stream(package->data);

        QList<Feature> feature;
        stream >> feature;
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
