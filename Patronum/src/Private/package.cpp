/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "package.h"

namespace Patronum {

Command Package::cmd() const {
    return static_cast<Command>(m_hdr.cmd);
}

const QByteArray &Package::data() const {
    return m_data;
}

bool Package::isValid() const {
    return m_hdr.isValid() && m_hdr.size == m_data.size();
}

void Package::reset() {
    m_hdr = {0, 0};
    m_data.clear();
}

Package::Package() {
    reset();
}

bool Header::isValid() const {
    return cmd && cmd <= static_cast<int>(Command::CloseConnection);
}


}
