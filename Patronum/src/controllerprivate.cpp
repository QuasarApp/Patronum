#include "controllerprivate.h"
#include "icontroller.h"
#include "localsocket.h"
#include <quasarapp.h>
#include "package.h"

namespace Patronum {

ControllerPrivate::ControllerPrivate(const QString &name, IController *controller, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name);
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

    stream << Command::FeaturesRequest;

    return _socket->send(responce);
}

bool ControllerPrivate::sendCmd(const QList<Feature> &result) {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Error);
        return false;
    }

    QByteArray responce;
    QDataStream stream(&responce, QIODevice::WriteOnly);

    stream << Command::Feature << result;

    return _socket->send(responce);
}

void ControllerPrivate::handleReceve(QByteArray data) {

    if (data.size() < 2) {
        return;
    }

    const Package *package = reinterpret_cast<const Package *>( data.data());

    switch (package->cmd) {

    case Command::Features: {

        if (!_controller) {
            QuasarAppUtils::Params::log("System error, controller is not inited!",
                                        QuasarAppUtils::Error);
            break;
        }

        QDataStream stream(package->data);

        QList<Feature> features;
        stream >> features;

        _controller->handleFeatures(features);

        break;

    }

    case Command::FeatureResponce: {
        if (!_controller) {
            QuasarAppUtils::Params::log("System error, controller is not inited!",
                                        QuasarAppUtils::Error);
            break;
        }

        QDataStream stream(package->data);

        QVariantMap feature;
        stream >> feature;
        _controller->handleResponce(feature);

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
