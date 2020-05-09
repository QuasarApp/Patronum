#ifndef ICONTROLER_H
#define ICONTROLER_H

#include <QList>
#include <QVariantMap>

namespace Patronum {

class Feature;

/**
 * @brief The ControllerError enum
 *  - Undefined - Unknown error
 *  - ServiceUnavailable - Service is unavailable. Try send start comand or restart the service manually.
 *  - InvalidPackage - Invalid package received
 *  - WrongCommand - Library unsupported command received
 *  - SystemError - Internal error of the work of the Patronum library. Contact the developers and provide them with an error report. https://github.com/QuasarApp/Patronum/issues
 */
enum class ControllerError {
    Undefined,
    ServiceUnavailable,
    InvalidPackage,
    WrongCommand,
    SystemError
};

/**
 * @brief The IController class
 */
class IController
{
public:
    IController() = default;
    /**
     * @brief errorToString this method convert the ControllerError to QString.
     * @param error - error id
     * @return return text of error
     */
    QString errorToString(ControllerError error) const;

    virtual void handleFeatures(const QList<Feature>& features) = 0;
    virtual void handleResponce(const QVariantMap& feature) = 0;
    virtual void handleError(ControllerError) = 0;

};
}

#endif // ICONTROLER_H
