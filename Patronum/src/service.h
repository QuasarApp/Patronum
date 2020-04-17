#ifndef SERVICE_H
#define SERVICE_H
#include "Patronum_global.h"
#include <qtservice.h>

namespace Patronum {
template<class Application>
class PATRONUM_LIBRARYSHARED_EXPORT Service : public QtService<Application>
{
public:
    Service();
};

}


#endif // SERVICE_H
