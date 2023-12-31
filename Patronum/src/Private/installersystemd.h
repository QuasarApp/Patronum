/*
 * Copyright (C) 2018-2024 QuasarApp.
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
    InstallerSystemD();
    ~InstallerSystemD() override;

    // Installer interface
public:
    bool install(const QString &executable, const QString &user = DEFAULT_USER) override;
    bool uninstall() override;
    bool enable() override;
    bool disable() override;
    bool isInstalled() const override;

private:
    QString absaluteServicePath() const;
};

}
#endif // INSTALLERSYSTEMD_H
