/*
 * Copyright (C) 2018-2027 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SOCKETWRAPER_H
#define SOCKETWRAPER_H

#include <QByteArray>


namespace Patronum {

enum class State: int {
    Connected,
    Disconnected
};

class ISocketWraper
{
public:
    ISocketWraper();
    /**
     * @brief send - send data to service
     * @param data
     * @return true if operation finished successful
     */
    virtual bool send(const QByteArray& data) = 0;

    /**
     * @brief sigReceve - this method is a prototype of received data signal
     * @param data - received data
     * @return
     */
    virtual void sigReceve(QByteArray data) = 0;

    /**
     * @brief sigStateChanged - this method is a prototype of change state signal
     * @param state
     */
    virtual void sigStateChanged(State state) = 0;


    /**
     * @brief isValid - check validation of socket
     */
    virtual bool isValid() const = 0;

    /**
     * @brief listen
     * @return true if a socket deploy successful
     */
    virtual bool listen() = 0;

    /**
     * @brief connectToTarget
     * @return true if a socket connect successful
     */
    virtual bool connectToTarget() = 0;

    /**
     * @brief isRunning This method should be return true if the service is running else false.
     * @return return true if the service is running else false.
     */
    virtual bool isRunning() = 0;

    State state() const;

    bool echo() const;
    void setEcho(bool newEcho);

protected:
    State m_state = State::Disconnected;
    bool _echo = true;

};

}




#endif // SOCKETWRAPER_H
