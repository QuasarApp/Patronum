#include "feature.h"

#include <QDataStream>

namespace Patronum {

Feature::Feature(const QString &cmd, const QVariantList &arg) {
    _cmd = cmd;
    _arg = arg;
}

QString Feature::cmd() const {
    return _cmd;
}

void Feature::setCmd(const QString &cmd) {
    _cmd = cmd;
}

QVariantList Feature::arg() const {
    return _arg;
}

void Feature::setArg(const QVariantList &arg) {
    _arg = arg;
}

QDataStream &operator<<(QDataStream &stream, const Feature &obj) {
    stream << obj._cmd << obj._arg;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, Feature &obj) {
    stream >> obj._cmd >> obj._arg;
    return stream;
}

}
