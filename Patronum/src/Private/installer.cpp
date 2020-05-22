/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "installer.h"
namespace Patronum {

Installer::Installer(const QString &name) {
    _serviceName = name;
}

QString Installer::serviceName() const {
    return _serviceName;
}

}

