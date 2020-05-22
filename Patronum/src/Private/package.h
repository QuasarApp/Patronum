/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PACKAGE_H
#define PACKAGE_H

#include <QDataStream>
#include <PFeature.h>
#include <QVariantMap>

namespace Patronum {

class Feature;

enum class Command: quint8 {
    FeaturesRequest,
    Features,
    Feature,
    FeatureResponce
};

/**
 * @brief The Package class
 * this is base package class with cmd and data
 */
class Package
{
public:
    Command cmd() const;
    QByteArray data() const;
    bool isValid() const;

    template<class DATA>
    static QByteArray createPackage(Command cmd, const DATA &data) {
        QByteArray result;
        QDataStream stream(&result, QIODevice::WriteOnly);

        stream << cmd;
        stream << data;

        return result;
    }
    static Package parsePackage(const QByteArray& data);

private:
    Package();

    unsigned char m_cmd;
    QByteArray m_data;
};

}

#endif // PACKAGE_H
