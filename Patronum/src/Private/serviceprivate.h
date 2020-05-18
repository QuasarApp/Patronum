#ifndef SERVICEPRIVATE_H
#define SERVICEPRIVATE_H
#include <QObject>
#include <PFeature.h>

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
    bool parseDefaultCmds();

    void listen() const;

private:
    LocalSocket *_socket = nullptr;
    IService  *_service = nullptr;

    bool hendleStandartCmd(QList<Feature> *cmds);

private slots:
    void handleReceve(QByteArray data);

};

}
#endif // SERVICEPRIVATE_H
