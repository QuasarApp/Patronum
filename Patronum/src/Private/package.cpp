/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

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
