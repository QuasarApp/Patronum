#ifndef SERVICE_H
#define SERVICE_H
#include "Patronum_global.h"
#include "feature.h"
#include "iservice.h"
#include "localsocket.h"
#include "package.h"
#include "serviceprivate.h"
#include <qtservice.h>

namespace Patronum {

template<class Application>
class PATRONUM_LIBRARYSHARED_EXPORT Service : public QtService<Application>, protected IService
{
public:
    Service(int argc, char **argv, const QString &name)
        : QtService<Application>(argc, argv, name) {
        d_ptr = new ServicePrivate(name, nullptr, this);

    }
// IService interface
protected:
    void handleReceve(const Feature &data) {
        Q_UNUSED(data)
    };

    QList<Feature> supportedFeatures() {
        return QList<Feature>();
    }

private:
    ServicePrivate *d_ptr = nullptr;
};

}


#endif // SERVICE_H
