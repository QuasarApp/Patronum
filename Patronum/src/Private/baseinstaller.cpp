/*
 * Copyright (C) 2018-2027 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "baseinstaller.h"
#include "pcommon.h"
#include <QFile>
#include <QSettings>
#include <quasarapp.h>

namespace Patronum {

BaseInstaller::BaseInstaller():
    Installer() {

}

bool BaseInstaller::install(const QString &executable, const QString &user) {

    Q_UNUSED(user)

    if (!QFile::exists(executable)) {
        qCritical() << "The service executable file is not exists " << executable;
        return false;
    }

    if (isInstalled()) {
        qInfo() << "The service " << PCommon::instance()->getServiceName() << " already installed";
        return false;
    }

    return true;
}

bool BaseInstaller::uninstall() {
    if (!isInstalled()) {

        qInfo() << "The service " << PCommon::instance()->getServiceName() << " already uninstalled";
        return false;
    }

    return true;
}

bool BaseInstaller::enable() {
    if (!isInstalled()) {

        qInfo() << QObject::tr("Cannot enabled the service %0 not installed, run install command before enable. \n").
                   arg(PCommon::instance()->getServiceName());

        return false;
    }

    return true;
}

bool BaseInstaller::disable() {
    if (!isInstalled()) {

        qInfo() << QObject::tr("Cannot disabled the service %0 not installed, run install command before enable. \n").
                  arg(PCommon::instance()->getServiceName());
        return false;
    }
    return true;
}
}
