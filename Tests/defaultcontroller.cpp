#include "defaultcontroller.h"

DefaultController::DefaultController():
    Patronum::Controller("TestPatronum") {

}

QVariantMap DefaultController::getResponce() {
    return _receiveData;
}

bool DefaultController::isFinished() const {
    return _finished;
}

void DefaultController::handleResponce(const QVariantMap &feature) {
    _receiveData = feature;
}

void DefaultController::finished() {
    _finished = true;
}
