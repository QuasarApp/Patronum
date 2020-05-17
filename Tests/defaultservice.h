#ifndef DEFAULTSERVICE_H
#define DEFAULTSERVICE_H
#include <patronum.h>
#include <QCoreApplication>

class DefaultService : public Patronum::Service<QCoreApplication>
{
public:
    DefaultService();

    // QtServiceBase interface
protected:
    void onStart();

    // IService interface
public:
    void handleReceive(const QList<Patronum::Feature> &data);
    QList<Patronum::Feature> supportedFeatures();
};

#endif // DEFAULTSERVICE_H
