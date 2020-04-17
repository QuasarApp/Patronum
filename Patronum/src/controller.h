#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Patronum_global.h"
#include <qtservice.h>

namespace Patronum {

class PATRONUM_LIBRARYSHARED_EXPORT Controller : public QtServiceController
{
public:
    Controller(const QString & name);
};
}
#endif // CONTROLLER_H
