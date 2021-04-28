#ifndef DEFAULTCONTROLLER_H
#define DEFAULTCONTROLLER_H
#include <patronum.h>

class DefaultController : public Patronum::Controller
{
public:
    DefaultController();
    QVariantMap getResponce();
    bool isFinished() const;

protected:
    void handleResponce(const QVariantMap &feature);
    void finished();

private:
    QVariantMap _receiveData;
    bool _finished = false;

};

#endif // DEFAULTCONTROLLER_H
