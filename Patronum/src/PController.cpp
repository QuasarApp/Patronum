#include "PController.h"
#include "controllerprivate.h"
#include <QDateTime>
#include <QVariantMap>
#include <quasarapp.h>

namespace Patronum {

Controller::Controller(const QString &name):
    QtServiceController(name) {
    d_ptr = new ControllerPrivate(name, this);
}

Controller::~Controller() {
    delete d_ptr;
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

        printDefaultHelp();

        return true;
    }

    if (QuasarAppUtils::Params::isEndable("start")) {
        return start();
    }

    if (QuasarAppUtils::Params::isEndable("stop")) {
        return stop();
    }

    if (QuasarAppUtils::Params::isEndable("resume")) {
        return resume();
    }

    if (QuasarAppUtils::Params::isEndable("pause")) {
        return pause();
    }

    if (QuasarAppUtils::Params::isEndable("install")) {
        return install(defaultInstallLocation());
    }

    if (QuasarAppUtils::Params::isEndable("uninstall")) {
        return uninstall();
    }

    QList<Feature> sendData = {};
    auto userParams = QuasarAppUtils::Params::getUserParamsMap();
    for (auto val = userParams.begin(); val != userParams.end(); ++val) {
        sendData += {val.key(), val.value()};
    }

    return d_ptr->sendCmd(sendData);
}

bool Controller::waitForResponce(int msec) {

    return d_ptr->waitForResponce(msec);
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

QString Controller::defaultInstallLocation() {
    return "";
}

QList<Feature> Controller::features() {
    return d_ptr->features();
}

void Controller::printDefaultHelp() const {
    QuasarAppUtils::Help::Charters help{{"General options of this controller",{
        {"start",    QObject::tr("Start a service")},
        {"stop",     QObject::tr("Stop a service")},
        {"pause",    QObject::tr("Pause a service")},
        {"resume",   QObject::tr("Resume a service")},
        {"install",  QObject::tr("Install a service")},
        {"uninstall", QObject::tr("Uninstall a service")}
    }}};

    QuasarAppUtils::Help::print(help);
}

}
