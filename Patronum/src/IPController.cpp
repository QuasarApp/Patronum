#include "IPController.h"

QString Patronum::IController::errorToString(Patronum::ControllerError error) const {
    switch (error) {
    case Patronum::ControllerError::ServiceUnavailable: return "Service is unavailable. Try send start comand or restart the service manually.";

    case Patronum::ControllerError::InvalidPackage: return "Invalid package received";

    case Patronum::ControllerError::WrongCommand: return "Library unsupported command received";

    case Patronum::ControllerError::SystemError: return "Internal error of the work of the Patronum library."
                                                        " Contact the developers and provide them with an error report."
                                                        " https://github.com/QuasarApp/Patronum/issues";

    default:
        return "Unknown error";
    }
}
