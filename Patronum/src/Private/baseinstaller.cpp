/*
 * Copyright (C) 2018-2021 QuasarApp.
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
        QuasarAppUtils::Params::log(QObject::tr("The service executable file is not exists %0\n").arg(executable),
                                    QuasarAppUtils::Error);
        return false;
    }

    if (isInstalled()) {
        QuasarAppUtils::Params::log(QObject::tr("the service %0 alredy installed \n").
                                    arg(PCommon::instance()->getServiceName()),
                                    QuasarAppUtils::Info);
        return false;
    }

    return true;
}

bool BaseInstaller::uninstall() {
    if (!isInstalled()) {
        QuasarAppUtils::Params::log(QObject::tr("the service %0 alredy uninstalled \n").
                                    arg(PCommon::instance()->getServiceName()),
                                    QuasarAppUtils::Info);
        return false;
    }

    return true;
}

bool BaseInstaller::enable() {
    if (!isInstalled()) {
        QuasarAppUtils::Params::log(QObject::tr("Cannot enabled the service %0 not installed, run install command befor enable. \n").
                                    arg(PCommon::instance()->getServiceName()),
                                    QuasarAppUtils::Info);
        return false;
    }

    return true;
}

bool BaseInstaller::disable() {
    if (!isInstalled()) {
        QuasarAppUtils::Params::log(QObject::tr("Cannot disabled the service %0 not installed, run install command befor enable. \n").
                                    arg(PCommon::instance()->getServiceName()),
                                    QuasarAppUtils::Info);
        return false;
    }
    return true;
}
}
