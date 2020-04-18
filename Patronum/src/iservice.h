#ifndef ISERVICE_H
#define ISERVICE_H

#include <QList>

namespace Patronum {

class Feature;

class IService
{
public:
    IService();

    virtual void handleReceve(const QList<Feature>& data) = 0;
    virtual QList<Feature> supportedFeatures() = 0;

};
}
#endif // ISERVICE_H
