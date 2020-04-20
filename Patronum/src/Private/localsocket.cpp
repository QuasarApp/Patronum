#include "localsocket.h"

#include <QLocalSocket>
namespace Patronum {

LocalSocket::LocalSocket(const QString &target) {
    m_socket = new QLocalSocket(this);
    m_socket->connectToServer(target);

    connect(m_socket, &QLocalSocket::stateChanged,
            this, &LocalSocket::handeStateChanged);

    connect(m_socket, &QLocalSocket::readyRead,
            this, &LocalSocket::handeReadyRead);

}

bool LocalSocket::send(const QByteArray &data) {
    if (!isValid()) {
        return false;
    }

    return m_socket->write(data) == data.size();
}

bool LocalSocket::isValid() const {
    return m_state == State::Connected;
}

bool LocalSocket::reconnect() {
    m_socket->connectToServer();
    return isValid();
}

void LocalSocket::handeStateChanged(QLocalSocket::LocalSocketState socketState) {
    if (socketState == QLocalSocket::LocalSocketState::ConnectedState) {
        m_state = State::Connected;
    } else {
        m_state = State::Disconeccted;
    }

    emit sigStateChanged(m_state);
}

void LocalSocket::handeReadyRead() {
    auto data = m_socket->readAll();
    emit sigReceve(data);
}
}
