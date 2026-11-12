/*
 * Copyright (C) 2018-2027 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef INSTALLER_H
#define INSTALLER_H

#include <QString>

#define DEFAULT_USER "root"

namespace Patronum {


/**
 * @brief The Installer class - base interface for all plugins
 */
class Installer
{
public:
    Installer();
    virtual ~Installer() = default;

    /**
     * @brief install This method install service on host.
     * @param executable This is path to service executable.
     * @param user This is user that will run service. If this argument is empty the service will be use root user.
     * @return true if service installed successful
     */
    virtual bool install(const QString& executable, const QString& user = DEFAULT_USER) = 0;

    /**
     * @brief uninstall This method remove service from autostart.
     * @return true if service removed successful
     */
    virtual bool uninstall() = 0;

    /**
     * @brief enable This method enable installed service. This method invoked by default in the Installer::install method.
     * @return true if the service enabled successfull.
     */
    virtual bool enable() = 0;

    /**
     * @brief disable This method disable installed service.
     * @return true if the service disabled successfull.
     */
    virtual bool disable() = 0;

    /**
     * @brief isInstalled This method return true if service already installed.
     * @return true if service already installed else false.
     */
    virtual bool isInstalled() const = 0;


};

}
#endif // INSTALLER_H
