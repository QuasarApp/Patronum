#ifndef INSTALLERSYSTEMD_H
#define INSTALLERSYSTEMD_H

#include "installer.h"

class QSettings;

namespace Patronum {

/**
 * @brief The InstallerSystemD class
 * this implementation install service on linux systems (systemD)
 */
class InstallerSystemD: public Installer
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
protected:
    void initService();
    QSettings * getSettings(const QString &serviceName);


};

}
#endif // INSTALLERSYSTEMD_H
