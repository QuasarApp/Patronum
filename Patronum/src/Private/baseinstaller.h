/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef BASEINSTALLER_H
#define BASEINSTALLER_H
#include "installer.h"

class QSettings;

namespace Patronum {

/**
 * @brief The BaseInstaller class - this class contains log for all overrides methods
 */
class BaseInstaller: public Installer
{
public:
    BaseInstaller();


    /**
     * @brief getExecutable This method return saved path to installed executable.
     * @return path to executable.
     */
    QString getExecutable() const override;

    /**
     * @brief install This implementation prepare executable for instalation.
     * @param executable This is path to executable file of service.
     * @return true if service is not installed else false
     */
    bool install(const QString &executable) override;

    /**
     * @brief uninstall This method check if service installed.
     * @return true if service not installed else false
     */
    bool uninstall() override;

    /**
     * @brief enable This implementation check if available service only.
     * @return true if It service may be enabled
     */
    bool enable() override;

    /**
     * @brief disable
     * @return true if It service may be disabled
     */
    bool disable() override;

protected:
    void savePath() const;
    QString getPath() const;

    QString _executable = "";


};
}
#endif // BASEINSTALLER_H
