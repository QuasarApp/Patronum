#ifndef FEaTURE_H
#define FEaTURE_H

#include <QString>
#include <QVariantList>
#include <QList>
#include <Patronum_global.h>

namespace Patronum {

/**
 * @brief The Feature class - it is atomic type for describe service command
 */

class PATRONUM_LIBRARYSHARED_EXPORT Feature
{
public:
    Feature() = default;
    Feature(const QString& cmd, const QVariantList& arg);
    QString cmd() const;
    void setCmd(const QString &cmd);

    QVariantList arg() const;
    void setArg(const QVariantList &arg);

    friend QDataStream& operator<<(QDataStream& stream, const Feature& obj);
    friend QDataStream& operator>>(QDataStream& stream, Feature& obj);

private:
    QString _cmd;
    QVariantList _arg;
};


}
#endif // FEaTURE_H
