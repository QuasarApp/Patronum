/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef FEaTURE_H
#define FEaTURE_H

#include <QString>
#include <QVariant>
#include <QList>
#include <Patronum_global.h>

namespace Patronum {

/**
 * @brief The Feature class it is atomic type for describe service command
 */
class PATRONUM_LIBRARYSHARED_EXPORT Feature
{
public:
    explicit Feature() = default;
    explicit Feature(const QString& cmd, const QString& arg = {},
            const QString& description = "", const QString& example = "");

    /**
     * @brief cmd This method return command of the feature
     * @return command of the feature
     */
    QString cmd() const;

    /**
     * @brief setCmd This method set new value of the command.
     * @param cmd
     */
    void setCmd(const QString &cmd);

    /**
     * @brief arg This method return argument value.
     *  The argument value has a qvariant type, so this object maybe have a list or array type.
     * @return argument value.
     */
    QString arg() const;
    void setArg(const QString &arg);

    PATRONUM_LIBRARYSHARED_EXPORT friend QDataStream& operator<<(QDataStream& stream, const Feature& obj);
    PATRONUM_LIBRARYSHARED_EXPORT friend QDataStream& operator>>(QDataStream& stream, Feature& obj);
    PATRONUM_LIBRARYSHARED_EXPORT friend bool operator==(const Feature& left, const Feature& right);

    /**
     * @brief description This method return description message of the command.
     * This string display in a teminal application in the help section.
     * @return description message of the command
     */
    QString description() const;

    /**
     * @brief setDescription This method sets description for command.
     * @param description tihs is new value of the description command.
     */
    void setDescription(const QString &description);

    /**
     * @brief example This is exmaple of using this command.
     * @return example of use.
     */
    QString example() const;

    /**
     * @brief setExample This method sets a new example
     * @param example This is a new example value.
     */
    void setExample(const QString &example);

    /**
     * @brief toString This is general method of the converting command to string.
     * @return string of all command.
     */
    QString toString() const;

    /**
     * @brief id This method retun id of the feature. The id is qHash from command string.
     * @return id of the feature.
     */
    unsigned int id() const;

private:

    unsigned int _id;
    QString _cmd;
    QString _description;
    QString _example;
    QString _arg;
};

uint PATRONUM_LIBRARYSHARED_EXPORT qHash(const Feature& feature);


}

#endif // FEaTURE_H
