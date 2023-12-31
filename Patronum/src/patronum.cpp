/*
 * Copyright (C) 2018-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include <QString>
#include "patronum.h"

namespace Patronum {

QString patronumLibVersion() {
    return PATRONUM_VERSION;
}

void init() {
    initPatronumResources();
}

}

