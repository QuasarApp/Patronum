#ifndef SERVICEPRIVATE_H
#define SERVICEPRIVATE_H
#include <QObject>
#include <feature.h>

namespace Patronum {

class LocalSocket;
class IService;
class IController;

class ServicePrivate: public QObject
{
    Q_OBJECT
public:
    ServicePrivate(const QString& name, IController* controller = nullptr,
                   IService* service = nullptr, QObject *parent = nullptr);

    bool sendCmdResult(const QVariantMap& result);
    bool sendFeaturesRequest();
    bool sendCmd(const QList<Feature>& result);

signals:
    void sigListFeatures(QList<Feature>);

private:
    LocalSocket *_socket = nullptr;
    IService  *_service = nullptr;
    IController  *_controller = nullptr;

private slots:
    void handleReceve(QByteArray data);
};

}
#endif // SERVICEPRIVATE_H
