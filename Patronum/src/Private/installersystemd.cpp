/*
 * Copyright (C) 2018-2025 QuasarApp.
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

bool InstallerSystemD::install(const QString &executable, const QString& user) {

    if (!BaseInstaller::install(executable)) {
        return false;
    }

    QString service;

    QFile templ(":/systemd/SystemD/service.service");
    QString name = PCommon::instance()->getServiceName();
    if (!templ.open(QIODevice::ReadOnly)) {

        qCritical() << "Cannot install " << name << ". The service template not available.";

        return false;
    }

    service = templ.readAll();
    templ.close();

    service = service.arg(executable);
    service = service.arg(PCommon::instance()->getPidfile(user));
    service = service.arg(PCommon::instance()->getPWD(user));
    service = service.arg(PCommon::instance()->getServiceName());

    if (user.isEmpty())
        service = service.arg(DEFAULT_USER);
    else
        service = service.arg(user);

    templ.setFileName(absaluteServicePath());

    if (!templ.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        qCritical() << "Cannot install " << name << ". " << templ.errorString();
        return false;
    }

    templ.write(service.toLatin1());

    templ.close();

    QProcess proc;
    proc.setProgram("systemctl");
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setArguments({"daemon-reload"});

    proc.start();

    return proc.waitForFinished() && enable();
}

bool InstallerSystemD::uninstall() {
    if (!BaseInstaller::uninstall()) {
        return false;
    }

    QString name = PCommon::instance()->getServiceName();

    if (!(disable() && QFile::remove(absaluteServicePath()))) {

        qCritical() << "Cannot uninstall " << name << ". Cannot remove " << absaluteServicePath() << ". Check permissions.";
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
    
