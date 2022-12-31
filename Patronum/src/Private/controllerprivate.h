/*
 * Copyright (C) 2018-2023 QuasarApp.
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
class Parser;

class ControllerPrivate: public QObject
{
    Q_OBJECT
public:
    ControllerPrivate(IController* controller = nullptr, QObject *parent = nullptr);
    ~ControllerPrivate();
    bool sendFeaturesRequest();
    bool sendCmd(const QSet<Feature> &result);

    bool stop();
    bool pause();
    bool resume();

    QList<Feature> features() const;
    bool isConnected() const;
    bool connectToHost(bool echo = true) const;

    void setEcho(bool echo);

signals:
    void sigListFeatures(QList<Feature>);

private slots:
    void handleReceve(QByteArray data);

private:
    LocalSocket *_socket = nullptr;
    IController *_controller = nullptr;
    QList<Feature> _features;
    Parser * _parser;
};

}
#endif // CONTROLLERPRIVATE_H
