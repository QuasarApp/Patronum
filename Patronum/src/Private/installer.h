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

protected:
    QString serviceName() const;

private:
    QString _serviceName = "";

};

}
#endif // INSTALLER_H
