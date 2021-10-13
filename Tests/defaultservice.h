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
    bool onStart() override;

    // IService interface
public:
    bool handleReceive(const Patronum::Feature &data) override;
    QSet<Patronum::Feature> supportedFeatures() override;
};

#endif // DEFAULTSERVICE_H
