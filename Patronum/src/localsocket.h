#ifndef LOCALSOCKET_H
#define LOCALSOCKET_H

#include "isocketwraper.h"

#include <QLocalSocket>
#include <QObject>

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
    LocalSocket(const QString& target);

    // ISocketWraper interface
public:
    bool send(const QByteArray &data);
    bool isValid() const;
    bool reconnect();

signals:
    void sigReceve(QByteArray data);
    void sigStateChanged(State state);

private:
    QLocalSocket *m_socket = nullptr;
private slots:
    void handeStateChanged(QLocalSocket::LocalSocketState socketState);
    void handeReadyRead();

};
}

#endif // LOCALSOCKET_H
