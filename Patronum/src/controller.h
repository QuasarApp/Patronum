#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Patronum_global.h"
#include "icontroller.h"
#include <qtservice.h>

namespace Patronum {

class ServicePrivate;

class PATRONUM_LIBRARYSHARED_EXPORT Controller : public QtServiceController, protected IController
{
public:
    Controller(const QString& name);

    // IControler interface
protected:
    void handleFeatures(const QList<Feature> &features);

private:
    ServicePrivate * d_ptr = nullptr;

};
}
#endif // CONTROLLER_H
