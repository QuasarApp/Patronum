/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SERVICEPRIVATE_H
#define SERVICEPRIVATE_H
#include <QObject>
#include <PFeature.h>

namespace Patronum {

class LocalSocket;
class IService;

class ServicePrivate: public QObject
{
    Q_OBJECT
public:
    ServicePrivate(const QString& name,
                   IService* service = nullptr, QObject *parent = nullptr);

    bool sendCmdResult(const QVariantMap& result);
    bool sendCloseConnection();

    bool parseDefaultCmds();

    void listen() const;

private:
    LocalSocket *_socket = nullptr;
    IService  *_service = nullptr;

    bool handleStandartCmd(QList<Feature> *cmds);

private slots:
    void handleReceve(QByteArray data);

};

}
#endif // SERVICEPRIVATE_H
