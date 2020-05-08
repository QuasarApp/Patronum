#ifndef LOCALSOCKET_H
#define LOCALSOCKET_H

#include "isocketwraper.h"

#include <QLocalSocket>
#include <QObject>

class QLocalServer;

namespace Patronum {

/**
 * @brief The LocalSocket class
 * this socket work only with locale data.
 */
class LocalSocket : public QObject, public ISocketWraper
{
    Q_OBJECT
public:
    /**
     * @brief LocalSocket
     * @param target - target it is localSocket name or ip:port
     */
    LocalSocket(const QString& target, QObject* ptr = nullptr);
    ~LocalSocket();

    // ISocketWraper interface
public:
    bool send(const QByteArray &data);
    bool isValid() const;
    bool listen();
    bool connectToTarget();

signals:
    void sigReceve(QByteArray data);
    void sigStateChanged(State state);

private:
    QLocalSocket *m_socket = nullptr;
    QLocalServer *m_server = nullptr;
    QString m_target;

    bool registerSokcet(QLocalSocket *socket);

private slots:
    void handleStateChanged(QLocalSocket::LocalSocketState socketState);
    void handleReadyRead();
    void handleIncomming();
    void handleSocketError(QLocalSocket::LocalSocketError);

};
}

#endif // LOCALSOCKET_H
