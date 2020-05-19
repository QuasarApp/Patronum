#include "installersystemd.h"
#include <QFile>
#include <quasarapp.h>
#include <QSettings>
#include <QProcess>
namespace Patronum {

const QString systemDPath = "/etc/systemd/system/";
static bool isServiceInited = false;

InstallerSystemD::InstallerSystemD(const QString& name):
    BaseInstaller(name) {

}

Patronum::InstallerSystemD::~InstallerSystemD() {

}

bool InstallerSystemD::install(const QString &executable) {

    if (BaseInstaller::install(executable)) {
        return true;
    }

    auto settings = getSettings(serviceName());

    settings->beginGroup("Unit");
    settings->setValue("Description", "Automatet generated service of " + executable);
    settings->endGroup();

    settings->beginGroup("Service");
    settings->setValue("Type", "forking");
    settings->setValue("User", "root");
    settings->setValue("Group", "root");
    settings->setValue("ExecStart", executable);
    settings->setValue("ExecStop", executable + " stop");
    settings->endGroup();

    settings->beginGroup("Install");
    settings->setValue("WantedBy", "multi-user.target");
    settings->endGroup();

    settings->sync();

    QSettings::Status ret = settings->status();

    if (ret == QSettings::AccessError) {

        QuasarAppUtils::Params::log(QString{"Cannot install %0. Cannot write to: %1. Check permissions.\n"}.
                                    arg(serviceName(), systemDPath + serviceName() + ".conf"),
                                    QuasarAppUtils::Error);

        return false;
    }

    bool renamed = QFile(systemDPath + serviceName() + ".conf").rename(
                absaluteServicePath());

    if (!renamed) {
        QuasarAppUtils::Params::log(QString{"Cannot install %0. Cannot rename service file to: %1. Check permissions.\n"}.
                                    arg(serviceName(), absaluteServicePath()),
                                    QuasarAppUtils::Error);

    }

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

QSettings *InstallerSystemD::getSettings(const QString& serviceName) {

    static QSettings* res = new QSettings(QSettings::SystemScope, serviceName);

    if (!isServiceInited) {
        res->setPath(QSettings::NativeFormat,
                           QSettings::SystemScope,
                           systemDPath);
        isServiceInited = true;
    }

    return res;
}

QString InstallerSystemD::absaluteServicePath() const {
    return systemDPath + serviceName() + ".service";
}

}
    
