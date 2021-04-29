/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef CONTROLLERPRIVATE_H
#define CONTROLLERPRIVATE_H
#include <PFeature.h>

namespace Patronum {

class IController;
class LocalSocket;
class Installer;
class Parser;

class ControllerPrivate: public QObject
{
    Q_OBJECT
public:
    ControllerPrivate(const QString& servicePath,
                      IController* controller = nullptr, QObject *parent = nullptr);
    ~ControllerPrivate();
    bool sendFeaturesRequest();
    bool sendCmd(const QSet<Feature> &result);

    int start() const;
    bool stop();
    bool install() const;
    bool uninstall() const;
    bool pause();
    bool resume();

    QList<Feature> features() const;

    bool isConnected() const;

    bool connectToHost() const;

signals:
    void sigListFeatures(QList<Feature>);

protected:
    QString getServiceExe() const;
    void setServiceExe(const QString &serviceExe);

private slots:
    void handleReceve(QByteArray data);

private:

    LocalSocket *_socket = nullptr;
    IController *_controller = nullptr;
    QList<Feature> _features;
    QString _serviceExe = "";
    Installer *_installer = nullptr;
    Parser * _parser;

};

}
#endif // CONTROLLERPRIVATE_H
