/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "installersystemd.h"
#include <QFile>
#include <quasarapp.h>
#include <QSettings>
#include <QProcess>
namespace Patronum {

const QString systemDPath = "/etc/systemd/system/";

InstallerSystemD::InstallerSystemD(const QString& name):
    BaseInstaller(name) {

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
                                    arg(serviceName(), absaluteServicePath()),
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
    proc.setArguments({"enable", serviceName() + ".service"});

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
    proc.setArguments({"disable", serviceName() + ".service"});

    proc.start();

    return proc.waitForFinished();
}

bool InstallerSystemD::isInstalled() const {
    return QFile::exists(absaluteServicePath());
}

QString InstallerSystemD::absaluteServicePath() const {
    return systemDPath + serviceName() + ".service";
}

}
    
