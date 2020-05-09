#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Patronum_global.h"
#include <IPController.h>
#include <qtservice.h>
#include "PFeature.h"

namespace Patronum {

class ControllerPrivate;

/**
 * @brief The Controller class provide control functionality for your service
 * ###How to use :
 * - just inherit from the Service Controller and override the methods you need.
 * - So, invoke the send methon, and if you need to get a responce from your service then invoke a waitForResponce method.
 */
class PATRONUM_LIBRARYSHARED_EXPORT Controller : public QtServiceController, protected IController
{
public:
    /**
     * @brief Controller - base constructor
     * @param name - name of you service
     */
    Controller(const QString& name);
    ~Controller() override;

    /**
     * @brief send - this method send request to service
     * @param argc - count of arguments
     * @param argv - arguments list
     * @return true if all sendet seccussful
     */
    bool send(int argc, char **argv);

    /**
     * @brief waitForResponce - waut for get a responce from servece
     * @param msec timeout
     * @return true if all seccussful
     */
    bool waitForResponce(int msec = 10000);

    // IControler interface
protected:

    /**
     * @brief handleFeatures - override this method if you want cerate a custom reaction of get service features
     * default inplenebtation prin help of available command of your service
     * @param features - list of features
     */
    void handleFeatures(const QList<Feature> &features) override;

    /**
     * @brief handleResponce - override this method if you want create a custom reaction of get responce from service
     * Default inplementation print responce to console.
     * @param responce - responce from service
     */
    void handleResponce(const QVariantMap &responce) override;

    /**
     * @brief defaultInstallLocation - this method must be return a path for service executable or wrapper
     *  Override this method if you want add support install option for controller
     */
    virtual QString defaultInstallLocation();

    /**
     * @brief features - this metho return current features of connected service.
     * @note If Responed from service not received then return empty list.
     * @return features list
     */
    QList<Feature> features();


private:
    ControllerPrivate *d_ptr = nullptr;

    void printDefaultHelp() const;

};
}
#endif // CONTROLLER_H
