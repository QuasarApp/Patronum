#include "pcommon.h"
#include "QCoreApplication"

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

QString PCommon::getPWD() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString PCommon::getServiceName() const {
    return QCoreApplication::applicationName();
}

}
