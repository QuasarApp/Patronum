#ifndef DEFAULTCONTROLLER_H
#define DEFAULTCONTROLLER_H
#include <patronum.h>

class DefaultController : public Patronum::Controller
{
public:
    DefaultController();
    QVariantMap getResponce();

protected:
    void handleResponce(const QVariantMap &feature);

private:
    QVariantMap _receiveData;


};

#endif // DEFAULTCONTROLLER_H
