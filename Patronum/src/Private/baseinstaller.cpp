#include "baseinstaller.h"
#include <QSettings>
#include <quasarapp.h>

namespace Patronum {

BaseInstaller::BaseInstaller(const QString &name):
    Installer(name)
{

}

bool BaseInstaller::install(const QString &executable) {
    _executable = executable;

    if (isInstalled()) {
        QuasarAppUtils::Params::log(QString("the service %0 alredy installed \n").
                                    arg(serviceName()),
                                    QuasarAppUtils::Info);
        return true;
    }

    return false;
}

bool BaseInstaller::uninstall() {
    if (!isInstalled()) {
        QuasarAppUtils::Params::log(QString("the service %0 alredy uninstalled \n").
                                    arg(serviceName()),
                                    QuasarAppUtils::Info);
        return true;
    }

    return false;
}

bool BaseInstaller::enable() {
    if (!isInstalled()) {
        QuasarAppUtils::Params::log(QString("Cannot enabled the service %0 not installed, run install command befor enable. \n").
                                    arg(serviceName()),
                                    QuasarAppUtils::Info);
        return false;
    }

    return true;
}

bool BaseInstaller::disable() {
    if (!isInstalled()) {
        QuasarAppUtils::Params::log(QString("Cannot disabled the service %0 not installed, run install command befor enable. \n").
                                    arg(serviceName()),
                                    QuasarAppUtils::Info);
        return false;
    }
    return true;
}

QString BaseInstaller::getPath() const {
    QSettings settings;

    return settings.value("ServicePath", "").toString();
}

QString BaseInstaller::getExecutable() const {
    return getPath();
}

void BaseInstaller::savePath() const{
    QSettings settings;

    settings.setValue("ServicePath", _executable);
    settings.sync();

}
}
