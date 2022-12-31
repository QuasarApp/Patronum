/*
 * Copyright (C) 2018-2023 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isocketwraper.h"

namespace Patronum {

ISocketWraper::ISocketWraper(){
}

State ISocketWraper::state() const {
    return m_state;
}

bool ISocketWraper::echo() const {
    return _echo;
}

void ISocketWraper::setEcho(bool newEcho) {
    _echo = newEcho;
}

}
