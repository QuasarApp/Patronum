/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "installersystemd.h"
#include "pcommon.h"
#include <QFile>
#include <quasarapp.h>
#include <QSettings>
#include <QProcess>
namespace Patronum {

#define SYSTEMD_PATH "/etc/systemd/system/"

InstallerSystemD::InstallerSystemD():
    BaseInstaller() {

}

Patronum::InstallerSystemD::~InstallerSystemD() {

}

bool InstallerSystemD::install(const QString &executable) {

    if (BaseInstaller::install(executable)) {
        return true;
    }

    QString service;

    QFile templ(":/systemd/SystemD/service.service");

    if (!templ.open(QIODevice::ReadOnly)) {
        QuasarAppUtils::Params::log(QString{"Cannot install %0. System error.\n"}.
                                    arg(executable),
                                    QuasarAppUtils::Error);
    }

    service = templ.readAll();
    templ.close();

    service = service.arg(executable);
    service = service.arg(PCommon::instance()->getPidfile());
    service = service.arg(PCommon::instance()->getPWD());

    templ.setFileName(absaluteServicePath());

    if (!templ.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QuasarAppUtils::Params::log(QString{"Cannot install %0. Cannot write to: %1. Check permissions.\n"}.
                                    arg(executable, absaluteServicePath()),
                                    QuasarAppUtils::Error);

    }

    templ.write(service.toLatin1());

    return enable();
}

bool InstallerSystemD::uninstall() {
    if (BaseInstaller::uninstall()) {
        return true;
    }

    if (!(disable() && QFile::remove(absaluteServicePath()))) {
        QuasarAppUtils::Params::log(QString("Cannot uninstall %0. Cannot remove %1. Check permissions.\n").
                                    arg(PCommon::instance()->getServiceName(),
                                        absaluteServicePath()),
                                    QuasarAppUtils::Error);
        return false;
    }
    return true;
}

bool InstallerSystemD::enable() {
    if (!BaseInstaller::enable()) {
        return false;
    }

    QProcess proc;
    proc.setProgram("systemctl");
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setArguments({"enable", PCommon::instance()->getServiceName() + ".service"});

    proc.start();

    return proc.waitForFinished();
}

bool InstallerSystemD::disable() {
    if (!BaseInstaller::disable()) {
        return false;
    }

    QProcess proc;
    proc.setProgram("systemctl");
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setArguments({"disable", PCommon::instance()->getServiceName() + ".service"});

    proc.start();

    return proc.waitForFinished();
}

bool InstallerSystemD::isInstalled() const {
    return QFile::exists(absaluteServicePath());
}

QString InstallerSystemD::absaluteServicePath() const {
    return SYSTEMD_PATH + PCommon::instance()->getServiceName() + ".service";
}

}
    
