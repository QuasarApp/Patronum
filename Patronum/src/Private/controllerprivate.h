#ifndef CONTROLLERPRIVATE_H
#define CONTROLLERPRIVATE_H
#include <PFeature.h>

namespace Patronum {

class IController;
class LocalSocket;
class Installer;

class ControllerPrivate: public QObject
{
    Q_OBJECT
public:
    ControllerPrivate(const QString& name, const QString& servicePath,
                      IController* controller = nullptr, QObject *parent = nullptr);
    ~ControllerPrivate();
    bool sendFeaturesRequest();
    bool sendCmd(const QList<Feature>& result);

    int start() const;
    bool stop();
    bool install() const;
    bool uninstall() const;
    bool pause();
    bool resume();

    bool waitForResponce(int msec);

    QList<Feature> features() const;

    bool isConnected() const;

    bool connectToHost() const;

signals:
    void sigListFeatures(QList<Feature>);

private:
    LocalSocket *_socket = nullptr;
    IController *_controller = nullptr;
    bool _responce = false;
    QList<Feature> _features;
    QString _serviceExe = "";
    Installer *_installer = nullptr;

private slots:
    void handleReceve(QByteArray data);
};

}
#endif // CONTROLLERPRIVATE_H
