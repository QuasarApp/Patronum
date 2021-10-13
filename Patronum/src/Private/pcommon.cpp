#include "pcommon.h"
#include "QCoreApplication"

#include <QFile>
#include <QStandardPaths>

namespace Patronum {

PCommon::PCommon() {}

const class PCommon *PCommon::instance() {
    static PCommon *inst = new PCommon();
    return inst;
}

QString PCommon::getPidfile() const {
    return getPWD() + "/" + getServiceName() + ".pid";
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

QString PCommon::getPWD() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString PCommon::getServiceName() const {
    return QCoreApplication::applicationName();
}

}
