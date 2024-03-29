#ifndef COMMON_H
#define COMMON_H

#include <QString>

namespace Patronum {

/**
 * @brief The Common class contains utils methods of the patronum library.
 *
 * ### Example
 *
 * @code{cpp}
 *  #include "pcommon.h"
 *  PCommon::instance()->getServiceName();
 * @endcode
 */
class PCommon {

public:
    /**
     * @brief instance This method return instance of the common object.
     * @return pointer to the common object.
     */
    static const PCommon* instance();

    /**
     * @brief pidFilePath This method return the absalute path to the service pid file.
     * @param customUser This is name of custom user for that will return PWD dir.
     * @return absalute path to the service pid file.
     */
    QString getPidfile(const QString& customUser = "") const;

    /**
     * @brief getPidFromPidfile This method return the pid from pidFile.
     * @return pid number from pidFile.
     */
    qint64 getPidFromPidfile() const;

    /**
     * @brief getPWD This method return PWD of the service.
     * @param customUser This is name of custom user for that will return PWD dir.
     * @return absalute path of the work directory.
     */
    QString getPWD(const QString &customUser = "") const;

    /**
     * @brief getServiceName return the service name.
     * @note To correct work of this method you need to invoke the     QCoreApplication::setApplicationName() methid.
     * @return application name
     */
    QString getServiceName() const;

private:
    PCommon();
};
}
#endif // COMMON_H
