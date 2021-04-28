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
class Parser;

/**
 * @brief The ServicePrivate class
 */
class ServicePrivate: public QObject
{
    Q_OBJECT
public:
    ServicePrivate(IService* service = nullptr, QObject *parent = nullptr);
    ~ServicePrivate();

    /**
     * @brief sendCmdResult
     * @param result
     * @return
     */
    bool sendCmdResult(const QVariantMap& result);

    /**
     * @brief sendCloseConnection
     * @return
     */
    bool sendCloseConnection();

    /**
     * @brief parseDefaultCmds
     * @return
     */
    bool parseDefaultCmds();

    /**
     * @brief listen This method run service and enable service sockets for listning clients.
     * @return true if dpeloy service finished successful else false.
     */
    bool listen() const;

private slots:
    void handleReceve(QByteArray data);

private:
    bool handleStandartCmd(QSet<Feature> *cmds);

    LocalSocket *_socket = nullptr;
    IService  *_service = nullptr;
    Parser *_parser = nullptr;


};

}
#endif // SERVICEPRIVATE_H
