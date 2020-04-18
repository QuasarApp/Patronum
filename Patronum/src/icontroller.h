#ifndef ICONTROLER_H
#define ICONTROLER_H

#include <QList>
#include <QVariantMap>

namespace Patronum {

class Feature;

class IController
{
public:
    IController();
    virtual void handleFeatures(const QList<Feature>& features) = 0;
    virtual void handleResponce(const QVariantMap& feature) = 0;

};
}

#endif // ICONTROLER_H
