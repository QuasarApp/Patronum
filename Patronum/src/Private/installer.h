/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef INSTALLER_H
#define INSTALLER_H

#include <QString>


namespace Patronum {

/**
 * @brief The Installer class - base interface for all plugins
 */
class Installer
{
public:
    Installer(const QString& name);
    virtual ~Installer() = default;

    virtual bool install(const QString& executable) = 0;
    virtual bool uninstall() = 0;

    virtual bool enable() = 0;
    virtual bool disable() = 0;

    virtual bool isInstalled() const = 0;
    virtual QString getExecutable() const = 0;

protected:
    QString serviceName() const;

private:
    QString _serviceName = "";

};

}
#endif // INSTALLER_H
