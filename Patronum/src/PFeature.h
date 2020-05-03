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
    Feature() = default;
    Feature(const QString& cmd, const QVariant& arg = {},
            const QString& description = "", const QString& example = "");
    QString cmd() const;
    void setCmd(const QString &cmd);

    QVariant arg() const;
    void setArg(const QVariantList &arg);

    friend QDataStream& operator<<(QDataStream& stream, const Feature& obj);
    friend QDataStream& operator>>(QDataStream& stream, Feature& obj);

    QString description() const;
    void setDescription(const QString &description);

    QString example() const;
    void setExample(const QString &example);

    QString toString() const;

private:
    QString _cmd;
    QString _description;
    QString _example;
    QVariant _arg;
};


}
#endif // FEaTURE_H
