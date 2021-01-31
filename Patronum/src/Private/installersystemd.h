/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef INSTALLERSYSTEMD_H
#define INSTALLERSYSTEMD_H

#include "baseinstaller.h"

class QSettings;

namespace Patronum {

/**
 * @brief The InstallerSystemD class
 * this implementation install service on linux systems (systemD)
 */
class InstallerSystemD: public BaseInstaller
{
public:
    /**
     * @brief InstallerSystemD
     * @param name - service name
     */
    InstallerSystemD(const QString &name);
    ~InstallerSystemD() override;

    // Installer interface
public:
    bool install(const QString &executable) override;
    bool uninstall() override;
    bool enable() override;
    bool disable() override;
    bool isInstalled() const override;

private:
    QString absaluteServicePath() const;

};

}
#endif // INSTALLERSYSTEMD_H
