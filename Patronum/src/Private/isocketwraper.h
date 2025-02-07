/*
 * Copyright (C) 2018-2025 QuasarApp.
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
    Disconeccted
};

class ISocketWraper
{
public:
    ISocketWraper();
    /**
     * @brief send - send data to service
     * @param data
     * @return true if operation finished seccussful
     */
    virtual bool send(const QByteArray& data) = 0;

    /**
     * @brief sigReceve - this method is a prototype of received data signal
     * @param data - received data
     * @return
     */
    virtual void sigReceve(QByteArray data) = 0;

    /**
     * @brief sigStateChanged - this method is a prototype of chnage state signal
     * @param state
     */
    virtual void sigStateChanged(State state) = 0;


    /**
     * @brief isValid - check validation of socket
     */
    virtual bool isValid() const = 0;

    /**
     * @brief listen
     * @return true if a scoket deploy seccusseful
     */
    virtual bool listen() = 0;

    /**
     * @brief connectToTarget
     * @return true if a scoket connect seccusseful
     */
    virtual bool connectToTarget() = 0;

    /**
     * @brief isRunning This method shold be return true if the service is running else false.
     * @return return true if the service is running else false.
     */
    virtual bool isRunning() = 0;

    State state() const;

    bool echo() const;
    void setEcho(bool newEcho);

protected:
    State m_state = State::Disconeccted;
    bool _echo = true;

};

}




#endif // SOCKETWRAPER_H
