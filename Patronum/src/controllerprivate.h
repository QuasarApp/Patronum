#ifndef CONTROLLERPRIVATE_H
#define CONTROLLERPRIVATE_H
#include <feature.h>

namespace Patronum {

class IController;
class LocalSocket;

class ControllerPrivate: public QObject
{
    Q_OBJECT
public:
    ControllerPrivate(const QString& name, IController* controller = nullptr, QObject *parent = nullptr);
    bool sendFeaturesRequest();
    bool sendCmd(const QList<Feature>& result);

signals:
    void sigListFeatures(QList<Feature>);

private:
    LocalSocket *_socket = nullptr;
    IController *_controller = nullptr;

private slots:
    void handleReceve(QByteArray data);
};

}
#endif // CONTROLLERPRIVATE_H
