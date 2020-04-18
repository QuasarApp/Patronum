#ifndef ICONTROLER_H
#define ICONTROLER_H

#include <QList>


namespace Patronum {

class Feature;

class IController
{
public:
    IController();
    virtual void handleFeatures(const QList<Feature>& features) = 0;
};
}

#endif // ICONTROLER_H
