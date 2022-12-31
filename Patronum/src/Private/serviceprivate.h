/*
 * Copyright (C) 2018-2023 QuasarApp.
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
class Installer;

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
     * @brief install This method install your service in thec current system.
     * @param user This is name of custom user that will be run your service after reboot system.
     * @return return true if the service installed succesful
     */
    bool install(const QString &user);

    /**
     * @brief uninstall This method unistall your service in thec current system.
     * @return return true if the service unistalled succesful
     */
    bool uninstall();

    /**
     * @brief start This method start your service on this process. (as a classic application).
     *  This method run service and enable service sockets for listning clients.
     * @return return true if service started successfull
     */
    bool start();

    /**
     * @brief start This method start your service detached of this process. (as a deamon)
     * @return return true if service started successfull
     */
    bool startDeamon();

    /**
     * @brief stop This method stop this service.
     */
    void stop();

private slots:
    void handleReceve(QByteArray data);


private:

    /**
     * @brief handleStandartCmd
     * @param cmds
     * @return
     */
    bool handleStandartCmd(QSet<Feature> *cmds);

    /**
     * @brief getServiceLauncher This method return path to launcher file.
     * This method return absalute path to launch script. This method use the **P_RUN_FILE** and **CQT_RUN_FILE** envirement variables.
     * @note If the **P_RUN_FILE** and **CQT_RUN_FILE** envirement variables not defined of defined with invalid pathes then this method return absalute path to the current executable.
     * @return return absalute path to launcher script or executable file if search of path failed.
     */
    QString getServiceLauncher() const;

    LocalSocket *_socket = nullptr;
    IService  *_service = nullptr;
    Parser *_parser = nullptr;
    Installer *_installer = nullptr;


};

}
#endif // SERVICEPRIVATE_H
