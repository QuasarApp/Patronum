#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Patronum_global.h"
#include "IPController.h"
#include <qtservice.h>

namespace Patronum {

class ControllerPrivate;

/**
 * @brief The Controller class provide control functionality for your service
 * how to use :
 * just inherit from the Service Controller and override the methods you need.
 * So, invoke the send methon, and if you need to get a responce from your service then invoke a waitForResponce method.
 */
class PATRONUM_LIBRARYSHARED_EXPORT Controller : public QtServiceController, protected IController
{
public:
    /**
     * @brief Controller - base constructor
     * @param name - name of you service
     */
    Controller(const QString& name);

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
    void handleFeatures(const QList<Feature> &features);

    /**
     * @brief handleResponce - override this method if you want create a custom reaction of get responce from service
     * Default inplementation print responce to console.
     * @param feature - responce from service
     */
    void handleResponce(const QVariantMap &feature);

private:
    ControllerPrivate *d_ptr = nullptr;
    QList<Feature> _features;
    bool _responce = false;

};
}
#endif // CONTROLLER_H
