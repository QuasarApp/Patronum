#include "PFeature.h"

#include <QDataStream>

namespace Patronum {

Feature::Feature(const QString &cmd, const QVariant &arg,
                 const QString &description, const QString &example) {
    _cmd = cmd;
    _arg = arg;
    _description = description;
    _example = example;
}

QString Feature::cmd() const {
    return _cmd;
}

void Feature::setCmd(const QString &cmd) {
    _cmd = cmd;
}

QVariant Feature::arg() const {
    return _arg;
}

void Feature::setArg(const QVariantList &arg) {
    _arg = arg;
}

QString Feature::description() const {
    return _description;
}

void Feature::setDescription(const QString &description) {
    _description = description;
}

QString Feature::example() const {
    return _example;
}

void Feature::setExample(const QString &example) {
    _example = example;
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
