#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include "package.h"

namespace Patronum {

/**
 * @brief The Parser class This is package parser of the Patronum packages.
 */
class Parser
{
public:
    Parser();

    /**
     * @brief parse This method parse input data array @a array to the @result packages.
     * @param array This is input data.
     * @param result This is result packages.
     * @return return true if parse finished successful
     */
    bool parse(const QByteArray& array, QList<Package>& result);

    template<class DATA>
    /**
     * @brief createPackage This method create a custom package with @a cmd and @a data
     * @param cmd This is package command.
     * @param data This is data object.
     * @return bytes arra of the package.
     */
    QByteArray createPackage(Command cmd, const DATA &data) {
        QByteArray result;
        QDataStream stream(&result, QIODevice::WriteOnly);

        Header hdr;
        hdr.cmd = static_cast<unsigned char>(cmd);

        stream << data;
        hdr.size = result.size();
        result.insert(0, reinterpret_cast<char*>(&hdr), sizeof (hdr));

        return result;
    }

private:
    Package _data;
    QByteArray _hdrArray;

};
}
#endif // PARSER_H
