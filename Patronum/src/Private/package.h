/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PACKAGE_H
#define PACKAGE_H

#include <QDataStream>
#include <PFeature.h>
#include <QVariantMap>
#include <QIODevice>

namespace Patronum {

class Feature;

/**
 * @brief The Command enum This is base commands for sending
 */
enum class Command: quint8 {
    /// This is invalid command
    Undefined,
    /// This comand is request for the available command of the service
    FeaturesRequest,
    /// This comand is response of the FeaturesRequest command
    Features,
    /// This is request to execute command
    Feature,
    /// This is response of the execute of command
    FeatureResponce,
    /// This command is finished command for the terminal. after receive this command terminal will closse connection.
    CloseConnection
};

/**
 * @brief The Header struct have as size 2 byte. maximus package size is 4095 bytes.
 */
struct Header {
    /**
     * @brief size This is size of package data (exclude header size).
     */
    unsigned short size: 12;

    /**
     * @brief cmd This is workCommand
     */
    unsigned char cmd: 4;

    bool isValid() const;
};

/**
 * @brief The Package class
 * this is base package class with cmd and data
 */
class Package
{
public:
    Package();
    /**
     * @brief cmd This method return command of the package.
     * @return command of the package.
     */
    Command cmd() const;

    /**
     * @brief data This method return referense to the bytes array of the pacakge data.
     * @return bytes array of the pacakge data.
     */
    const QByteArray& data() const;

    /**
     * @brief isValid This method check this pacakge.
     * @return true if the package is valid.
     */
    bool isValid() const;

    /**
     * @brief reset This method reset all fields of the package to default value.
     * @note after invoke this method package will be invalid.
     */
    void reset();

private:
    Header m_hdr;
    QByteArray m_data;

    friend class Parser;
};

}

#endif // PACKAGE_H
