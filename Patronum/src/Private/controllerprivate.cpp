#include "controllerprivate.h"
#include "IPController.h"
#include "localsocket.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QProcess>
#include <quasarapp.h>
#include "package.h"
#include "installersystemd.h"

namespace Patronum {

ControllerPrivate::ControllerPrivate(const QString &name, const QString &servicePath,
                                     IController *controller, QObject *parent):
    QObject(parent) {
    _socket = new LocalSocket(name, this);
    _serviceExe = servicePath;
    _controller = controller;

#ifdef Q_OS_LINUX
    _installer = new InstallerSystemD(name);
#endif

    QObject::connect(_socket, &LocalSocket::sigReceve,
                     this, &ControllerPrivate::handleReceve);

}

ControllerPrivate::~ControllerPrivate() {
    if (_installer) {
        delete _installer;
    }
}

bool ControllerPrivate::sendFeaturesRequest() {
    if (!_socket->isValid()) {
        QuasarAppUtils::Params::log("scoket is closed!",
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::ServiceUnavailable);
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
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::ServiceUnavailable);

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

int ControllerPrivate::start() const {

    QProcess proc;
    proc.setProgram(_serviceExe);
    if (_installer && _installer->isInstalled() && _installer->getExecutable().size()) {
        proc.setProgram(_installer->getExecutable());
    }

    proc.setArguments({"exec"});
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setProcessChannelMode(QProcess::SeparateChannels);

    if (!proc.startDetached()) {
        QuasarAppUtils::Params::log("fail to start detached process: " + proc.errorString(),
                                    QuasarAppUtils::Error);
        return  static_cast<int>(ControllerError::SystemError);
    }

    _controller->handleResponce({{"Result", "start service successful"}});

    return 0;
}

bool ControllerPrivate::stop() {
    return sendCmd({Feature("stop")});
}

bool ControllerPrivate::install() const {
    if (!_installer) {
        QuasarAppUtils::Params::log("Unsupported platform",
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!_installer->install(_serviceExe)) {
        return false;
    }

    _controller->handleResponce({{"Result", "Install service successful"}});

    return true;
}

bool ControllerPrivate::uninstall() const {
    if (!_installer) {
        QuasarAppUtils::Params::log("Unsupported platform",
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!_installer->uninstall()) {
        return false;
    }

    _controller->handleResponce({{"Result", "Uninstall service successful"}});

    return _installer->uninstall();
}

bool ControllerPrivate::pause() {
    return sendCmd({Feature("pause")});
}

bool ControllerPrivate::resume() {
    return sendCmd({Feature("resume")});
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
    return _socket->isValid();
}

bool ControllerPrivate::connectToHost() const {
    if (isConnected()) {
        return true;
    }

    if (!_socket->connectToTarget()) {
        QuasarAppUtils::Params::log("Connect to service fail !",
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::ServiceUnavailable);

        return false;

    };

    return true;
}

void ControllerPrivate::handleReceve(QByteArray data) {

    const Package package = Package::parsePackage(data);

    if (!package.isValid()) {

        QuasarAppUtils::Params::log("Received invalid package!",
                                    QuasarAppUtils::Debug);

        _controller->handleError(ControllerError::InvalidPackage);

        return;
    }

    switch (package.cmd()) {

    case Command::Features: {

        if (!_controller) {
            QuasarAppUtils::Params::log("System error, controller is not inited!",
                                        QuasarAppUtils::Debug);
            _controller->handleError(ControllerError::SystemError);

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
            _controller->handleError(ControllerError::SystemError);

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
                                    QuasarAppUtils::Debug);
        _controller->handleError(ControllerError::WrongCommand);

        break;
    }

    }

}
}
