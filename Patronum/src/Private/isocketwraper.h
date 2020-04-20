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
    ISocketWraper(){};
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
     * @brief reconnect
     * @return true if socket connected to host;
     */
    virtual bool reconnect() = 0;

    State state() const;

protected:
    State m_state = State::Disconeccted;

};

}




#endif // SOCKETWRAPER_H
