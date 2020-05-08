#ifndef CONTROLLERPRIVATE_H
#define CONTROLLERPRIVATE_H
#include <PFeature.h>

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

    bool waitForResponce(int msec);

    QList<Feature> features() const;

    bool isConnected() const;

signals:
    void sigListFeatures(QList<Feature>);

private:
    LocalSocket *_socket = nullptr;
    IController *_controller = nullptr;
    bool _responce = false;
    QList<Feature> _features;


private slots:
    void handleReceve(QByteArray data);
};

}
#endif // CONTROLLERPRIVATE_H
