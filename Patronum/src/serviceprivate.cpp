#include "serviceprivate.h"

#include "iservice.h"
#include "icontroller.h"
#include "localsocket.h"
#include "package.h"

namespace Patronum {

Patronum::ServicePrivate::ServicePrivate(const QString &name, IController *controller, IService *service, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name);
    _service = service;
    _controller = controller;

    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ServicePrivate::handleReceve);

}

void ServicePrivate::handleReceve(QByteArray data) {

    if (data.size() < 2) {
        return;
    }

    const Package *package = reinterpret_cast<const Package *>( data.data());

    switch (package->cmd) {

    case Command::FeaturesRequest: {

        if (!_service) {
            // to-do logs
            break;
        }

        if (!_socket->isValid()) {
            // to-do logs
            break;
        }

        QList<Feature> features = _service->supportedFeatures();
        QByteArray sendData;
        QDataStream stream(&sendData, QIODevice::WriteOnly);

        stream << Command::Features << features;
        if (!_socket->send(sendData)) {
            // to-do logs
        }

        break;

    }
    case Command::Features: {

        if (!_controller) {
            // to-do logs
            break;
        }

        QDataStream stream(package->data);

        QList<Feature> features;
        stream >> features;

        _controller->handleFeatures(features);

        break;

    }
    case Command::Feature: {
        if (!_service) {
            // to-do logs
            break;
        }

        QDataStream stream(package->data);

        Feature feature;
        stream >> feature;
        _service->handleReceve(feature);

        break;

    }
    default: {
        // to-do add logs
        break;
    }

    }

}

}
