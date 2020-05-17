#include "installer.h"
namespace Patronum {

Installer::Installer(const QString &name) {
    _serviceName = name;
}

QString Installer::serviceName() const {
    return _serviceName;
}

}

