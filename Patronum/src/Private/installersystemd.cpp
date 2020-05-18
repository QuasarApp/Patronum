#include "installersystemd.h"
#include <QFile>
#include <quasarapp.h>
#include <QSettings>
#include <QProcess>
namespace Patronum {

const QString systemDPath = "/etc/systemd/system/";
static bool isServiceInited = false;

InstallerSystemD::InstallerSystemD(const QString& name):
    Installer(name) {

}

Patronum::InstallerSystemD::~InstallerSystemD() {

}

bool InstallerSystemD::install(const QString &executable) {

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

    bool renamed = QFile(systemDPath + serviceName() + ".conf").rename(
                systemDPath + serviceName() + ".service");

    QSettings::Status ret = settings->status();
    if (ret == QSettings::AccessError || !renamed) {

        QuasarAppUtils::Params::log(QString{"Cannot install %0. Cannot write to: %1. Check permissions.\n"}.
                                    arg(serviceName(), systemDPath + serviceName() + ".service"),
                                    QuasarAppUtils::Error);

        return false;
    }

    return enable();
}

bool InstallerSystemD::uninstall() {
    if (!(disable() && QFile::remove(systemDPath + serviceName() + ".service"))) {
        QuasarAppUtils::Params::log(QString("Cannot uninstall %0. Cannot remove %1. Check permissions.\n").
                                    arg(serviceName(), systemDPath + serviceName() + ".service"),
                                    QuasarAppUtils::Error);
        return false;
    }
    return true;
}

bool InstallerSystemD::enable() {
    QProcess proc;
    proc.setProgram("systemctl");
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setArguments({"enable", serviceName() + ".service"});

    proc.start();

    return proc.waitForFinished();
}

bool InstallerSystemD::disable() {
    QProcess proc;
    proc.setProgram("systemctl");
    proc.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc.setArguments({"disable", serviceName() + ".service"});

    proc.start();

    return proc.waitForFinished();
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

}
    
