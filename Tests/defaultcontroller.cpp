#include "defaultcontroller.h"

DefaultController::DefaultController():
    Patronum::Controller() {

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
