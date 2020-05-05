#ifndef SERVICE_H
#define SERVICE_H
#include "Patronum_global.h"
#include "PFeature.h"
#include "serviceprivate.h"
#include <IPService.h>
#include <qtservice.h>
#include <quasarapp.h>

namespace Patronum {

template<class Application>
/**
 * @brief The Service class it is  class for create a services from daemons.
 * ###How to use :
 * - just inherit from the Service class and override the methods you need.
 */
class PATRONUM_LIBRARYSHARED_EXPORT Service : public QtService<Application>, protected IService
{
public:
    /**
     * @brief Service
     * @param argc - count params
     * @param argv - test of arguments
     * @param name - name of your service
     */
    Service(int argc, char **argv, const QString &name)
        : QtService<Application>(argc, argv, name) {
        d_ptr = new ServicePrivate(name, this);

    }
// IService interface
protected:
    /**
     * @brief handleReceive - this method invoce when service receive new command from terminal of controller of this service
     * @param data - is list of commands from controller
     * Default inplementation send message abount error.
     */
    void handleReceive(const QList<Feature> &data) {
        Q_UNUSED(data)

        auto list = supportedFeatures();

        QStringList stringList;

        for (const auto&i : list) {
            stringList += i.toString();
        }

        QVariantMap result;

        result["Error"] = "Wrong command!";
        result["Available commands"] = stringList;

        sendResuylt(result);

    };

    /**
     * @brief supportedFeatures
     * @return list of supported features of this service. override this method for correctly work of your pair (service and controller)
     */
    QList<Feature> supportedFeatures() {
        QList<Feature> result;
        return result;
    }

    /**
     * @brief sendResuylt - call this method for send responce from service to tour controller
     * @param result
     * @return true if data sendet is seccusseful
     */
    bool sendResuylt(const QVariantMap &result) {
        return d_ptr->sendCmdResult(result);
    }

    /**
     * @brief createApplication default implementation create a Application object and parse argumnts.
     * @param argc argumnts count
     * @param argv list of argumnts
     */
    void createApplication(int argc, char **argv) {
        QuasarAppUtils::Params::parseParams(argc, argv);
        QtService<Application>::createApplication(argc, argv);
    }

private:
    ServicePrivate *d_ptr = nullptr;


};

}


#endif // SERVICE_H
