/*
 * Copyright (C) 2018-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "PFeature.h"

#include <QDataStream>

namespace Patronum {

Feature::Feature(const QString &cmd, const QString &arg,
                 const QString &description, const QString &example) {
    setCmd(cmd);
    setArg(arg);
    setDescription(description);
    setExample(example);
}

QString Feature::cmd() const {
    return _cmd;
}

void Feature::setCmd(const QString &cmd) {
    _cmd = cmd;
    _id = qHash(_cmd);
}

QString Feature::arg() const {
    return _arg;
}

void Feature::setArg(const QString &arg) {
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

QString Feature::toString() const {
    if (_description.size())
        return _cmd + ": " + _description;

    return _cmd;
}

unsigned int Feature::id() const {
    return _id;
}

QDataStream &operator<<(QDataStream &stream, const Feature &obj) {
    stream << obj._cmd << obj._arg;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, Feature &obj) {
    decltype (obj._cmd) cmd;
    stream >> cmd >> obj._arg;
    obj.setCmd(cmd);

    return stream;
}

bool operator==(const Feature &left, const Feature &right) {
    return left.id() == right.id();
}


uint qHash(const Feature &feature) {
    return feature.id();
}

}


