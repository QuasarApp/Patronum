#include "controller.h"
#include "serviceprivate.h"
namespace Patronum {

Controller::Controller(const QString &name):
    QtServiceController(name) {
    d_ptr = new ServicePrivate(name, this);
}

void Controller::handleFeatures(const QList<Feature> &features) {

}

}
