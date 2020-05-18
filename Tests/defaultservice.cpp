#include "defaultservice.h"
#include <QCoreApplication>

char* arg[] = {
    const_cast<char*>("/"),
    const_cast<char*>("exec")
};
int argc = 2;

DefaultService::DefaultService():
    Patronum::Service<QCoreApplication>(2, arg, "TestPatronum") {
    _core = new QCoreApplication(argc, arg);
}

void DefaultService::onStart() {
    QuasarAppUtils::Params::log("Server started!", QuasarAppUtils::Info);
}

void DefaultService::handleReceive(const QList<Patronum::Feature> &data) {

    QList<Patronum::Feature> notSupportedList;
    for (const auto& i : data) {
        if (i.cmd() == "ping") {
            sendResuylt("pong");
        } else {
            notSupportedList += i;
        }
    }

    Patronum::Service<QCoreApplication>::handleReceive(notSupportedList);
}

QList<Patronum::Feature> DefaultService::supportedFeatures() {
    QList<Patronum::Feature> res;
    res += Patronum::Feature("ping", {}, "test ping", "ping");

    return res;
}
