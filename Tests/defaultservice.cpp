#include "defaultservice.h"

const char* arg[] = {
    "/",
    "-e"
};

DefaultService::DefaultService():
    Patronum::Service<QCoreApplication>(0, arg, "TestPatronum") {

}

void DefaultService::start() {
    QuasarAppUtils::Params::log("Server started!", QuasarAppUtils::Info);
}

void DefaultService::handleReceive(const QList<Patronum::Feature> &data) {

    QList<Patronum::Feature> notSupportedList;
    for (const auto& i : data) {
        if (i.cmd() == "ping") {
            sendResuylt("pomg");
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
