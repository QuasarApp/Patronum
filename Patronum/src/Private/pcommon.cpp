#include "pcommon.h"
#include "QCoreApplication"

#include <QFile>
#include <QStandardPaths>
#include "quasarapp.h"

namespace Patronum {

PCommon::PCommon() {}

const class PCommon *PCommon::instance() {
    static PCommon *inst = new PCommon();
    return inst;
}

QString PCommon::getPidfile(const QString& customUser) const {
    return getPWD(customUser) + "/" + getServiceName() + ".pid";
}

qint64 PCommon::getPidFromPidfile() const {
    QFile file = getPidfile();
    if (!file.open(QIODevice::ReadOnly)) {
        return 0;
    }

    auto data = file.readAll();

    file.close();

    return data.toLongLong();
}

QString PCommon::getPWD(const QString& customUser) const {
    if (!customUser.size())
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

#ifdef Q_OS_LINUX
    if (customUser == "root")
        return "/root/.local/share/" + getServiceName();

    return "/home/" + customUser + "/.local/share/" + getServiceName();
#else

    QuasarAppUtils::Params::log("The custom user not support for " +
                                QSysInfo::kernelType(),
                                QuasarAppUtils::Error);

    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#endif
}

QString PCommon::getServiceName() const {
    return QCoreApplication::applicationName();
}

}
