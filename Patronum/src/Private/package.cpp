#include "package.h"

namespace Patronum {

Command Package::cmd() const {
    return static_cast<Command>(m_cmd);
}

QByteArray Package::data() const {
    return m_data;
}

bool Package::isValid() const {
    return m_cmd <= static_cast<int>(Command::FeatureResponce);
}

Package::Package() {
}

Package Package::parsePackage(const QByteArray &data) {
    if (!data.size()) {
        return {};
    }

    Package pkg;
    pkg.m_cmd = static_cast<unsigned char>(data.at(0));
    pkg.m_data = data.right(data.size() - sizeof (pkg.m_cmd));
    return pkg;
}

}
