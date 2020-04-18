#include "controller.h"
#include "serviceprivate.h"
#include <QDateTime>
#include <QVariantMap>
#include <quasarapp.h>

namespace Patronum {

Controller::Controller(const QString &name):
    QtServiceController(name) {
    d_ptr = new ServicePrivate(name, this);
}

bool Controller::send(int argc, char **argv) {
    if (!QuasarAppUtils::Params::parseParams(argc, argv)) {
        return false;
    }

    if (!QuasarAppUtils::Params::customParamasSize() ||
            QuasarAppUtils::Params::isEndable("h") ||
            QuasarAppUtils::Params::isEndable("help")) {

        if (!d_ptr->sendFeaturesRequest()) {
            return false;
        }

        return true;
    }

    QList<Feature> sendData = {};
    auto userParams = QuasarAppUtils::Params::getUserParamsMap();
    for (auto val = userParams.begin(); val != userParams.end(); ++val) {
        sendData += {val.key(), val.value()};
    }

    return d_ptr->sendCmd(sendData);
}

bool Controller::waitForResponce(int msec) {
    _responce = false;

    qint64 waitFor = QDateTime::currentMSecsSinceEpoch() + msec;

    while (!_responce && QDateTime::currentMSecsSinceEpoch() < waitFor) {
        QCoreApplication::processEvents();
    }

    return _responce;
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

    _features = features;
    QuasarAppUtils::Help::print(options);

    _responce = true;
}

void Controller::handleResponce(const QVariantMap &responce) {
    QuasarAppUtils::Help::Options options;

    for(auto iter = responce.begin(); iter != responce.end(); ++iter) {
        options.insert(iter.key(), iter.value().toString());
    }

    QuasarAppUtils::Help::print(options);

}

}
