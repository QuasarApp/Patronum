#ifndef DEFAULTSERVICE_H
#define DEFAULTSERVICE_H
#include <patronum.h>


class DefaultService : public Patronum::Service<QCoreApplication>
{
public:
    DefaultService();

    // QtServiceBase interface
protected:
    void start();

    // IService interface
public:
    void handleReceive(const QList<Patronum::Feature> &data);
    QList<Patronum::Feature> supportedFeatures();
};

#endif // DEFAULTSERVICE_H
