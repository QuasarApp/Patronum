/*
 * Copyright (C) 2018-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "localsocket.h"

#include <QFile>
#include <QLocalServer>
#include <QLocalSocket>
#include <quasarapp.h>

namespace Patronum {

LocalSocket::LocalSocket(const QString &target, QObject *ptr):
    QObject(ptr), ISocketWraper() {
    m_target = "P" + target;
}

LocalSocket::~LocalSocket() {
    if (m_server) {
        m_server->close();
        delete m_server;
    }

    if (m_socket) {
        m_socket->close();
        delete  m_socket;
    }
}

bool LocalSocket::registerSokcet(QLocalSocket *socket) {
    m_socket = socket;

    if (m_socket->parent() != this)
        m_socket->setParent(this);

    connect(m_socket, &QLocalSocket::stateChanged,
            this, &LocalSocket::handleStateChanged);

    connect(m_socket, &QLocalSocket::readyRead,
            this, &LocalSocket::handleReadyRead);

    connect(m_socket, qOverload<QLocalSocket::LocalSocketError>(&QLocalSocket::errorOccurred),
            this, &LocalSocket::handleSocketError);

    handleStateChanged(m_socket->state());

    return true;
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

bool LocalSocket::listen() {
    if (!m_server) {
        m_server = new QLocalServer(this);

        connect(m_server, &QLocalServer::newConnection,
                this, &LocalSocket::handleIncomming);
    }

    if (!m_server->removeServer(m_target)) {
        return false;
    }

    if (!m_server->listen(m_target)) {
        qCritical() << "Can't start local server! " << m_server->errorString();
        return false;
    }

    return true;
}

bool LocalSocket::connectToTarget() {

    if (!m_socket) {
        m_socket = new QLocalSocket(this);
        if (!registerSokcet(m_socket)) {

            qCritical() << "registerSokcet is failed!";
            return false;
        }
    }

    m_socket->connectToServer(m_target);

    return m_socket->isValid();
}

bool LocalSocket::isRunning() {
    return (m_server && m_server->isListening()) || QFile::exists(m_target);
}

void LocalSocket::handleStateChanged(QLocalSocket::LocalSocketState socketState) {

    if (socketState == QLocalSocket::LocalSocketState::ConnectedState) {
        m_state = State::Connected;
    } else {
        m_state = State::Disconeccted;
    }

    emit sigStateChanged(m_state);
}

void LocalSocket::handleReadyRead() {
    auto data = m_socket->readAll();
    emit sigReceve(data);
}

void LocalSocket::handleIncomming() {
    if (m_socket) {
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    registerSokcet(m_server->nextPendingConnection());
}

void LocalSocket::handleSocketError(QLocalSocket::LocalSocketError) {

    if (!_echo)
        return;

    auto _sender = dynamic_cast<QLocalSocket*>(sender());

    if (!_sender) {
        qCritical() << "Unknown error occurred!";
        return;
    }

    qWarning() << "Socket connection fail: " << _sender->errorString();

}

}
