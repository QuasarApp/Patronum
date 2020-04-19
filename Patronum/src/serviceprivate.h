#ifndef SERVICEPRIVATE_H
#define SERVICEPRIVATE_H
#include <QObject>
#include <feature.h>

namespace Patronum {

class LocalSocket;
class IService;

class ServicePrivate: public QObject
{
    Q_OBJECT
public:
    ServicePrivate(const QString& name,
                   IService* service = nullptr, QObject *parent = nullptr);

    bool sendCmdResult(const QVariantMap& result);

private:
    LocalSocket *_socket = nullptr;
    IService  *_service = nullptr;

private slots:
    void handleReceve(QByteArray data);
};

}
#endif // SERVICEPRIVATE_H
