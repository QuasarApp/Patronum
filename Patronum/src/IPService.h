/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ISERVICE_H
#define ISERVICE_H

#include <QList>

namespace Patronum {

class Feature;

class IService
{
public:
    IService();
    virtual ~IService() = default;

    virtual void handleReceive(const QList<Feature>& data) = 0;
    virtual QList<Feature> supportedFeatures() = 0;
    virtual void onStart() = 0;
    virtual void onStop() = 0;
    virtual void onResume() = 0;
    virtual void onPause() = 0;
};
}
#endif // ISERVICE_H
