#include "defaultservice.h"
#include <QCoreApplication>

char* arg[] = {
    const_cast<char*>("/"),
    const_cast<char*>("s")
};
int argc = 2;

DefaultService::DefaultService():
    Patronum::Service<QCoreApplication>(2, arg) {
    setCore(new QCoreApplication(argc, arg));
}

void DefaultService::onStart() {
    QuasarAppUtils::Params::log("Server started!", QuasarAppUtils::Info);
}

bool DefaultService::handleReceive(const Patronum::Feature &data) {

    if (data.cmd() == "ping") {
        sendResuylt("pong");

        return true;
    }

    return false;
}

QSet<Patronum::Feature> DefaultService::supportedFeatures() {
    QSet<Patronum::Feature> res;
    res += Patronum::Feature("ping", {}, "test ping", "ping");

    return res;
}
