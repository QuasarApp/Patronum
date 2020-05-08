#include "defaultcontroller.h"

DefaultController::DefaultController():
    Patronum::Controller("TestPatronum") {

}

QVariantMap DefaultController::getResponce() {
    return _receiveData;
}

void DefaultController::handleResponce(const QVariantMap &feature) {
    _receiveData = feature;
}
