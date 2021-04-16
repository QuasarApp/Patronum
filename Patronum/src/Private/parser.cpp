#include "parser.h"
#include <quasarapp.h>
namespace Patronum {

Parser::Parser() { }

bool Parser::parse(const QByteArray &array,
                   QList<Package> &result) {

    if (!array.size()) {
        return false;
    }

    int workIndex = 0;
    const int headerSize = sizeof(Header);

    const auto workArray = _hdrArray + array;
    const int arraySize = workArray.size();

    while (arraySize > workIndex) {
        int offset = arraySize - workIndex;
        if (_data.m_hdr.isValid()) {
            // CASE 1: The Package data is still not collected, but the header is already collected. performs full or partial filling of packet data.

            int dataLength = std::min(static_cast<int>(_data.m_hdr.size - _data.m_data.size()),
                                               arraySize - workIndex);
            _data.m_data.append(array.mid(workIndex + headerSize, dataLength));

            workIndex += dataLength;


        } else if (offset >= headerSize) {

            // CASE 2: The header and package still do not exist and the amount of data allows you to create a new header. A header is created and will fill in all or part of the package data.

            _data.reset();

            memcpy(&_data.m_hdr,
                   array.data() + workIndex, headerSize);

            int dataLength = std::min(static_cast<int>(_data.m_hdr.size),
                                      arraySize - headerSize - workIndex);

            _data.m_data.append(array.mid(workIndex + headerSize, dataLength));
            workIndex += headerSize + dataLength;

        } else {
            // CASE 3: There is not enough data to initialize the header. The data will be placed in temporary storage and will be processed the next time the data is received.

            unsigned char dataLength = static_cast<unsigned char>(arraySize - workIndex);
            _hdrArray += array.mid(workIndex, dataLength);
            workIndex += dataLength;
        }

        if (_data.isValid()) {
            result.push_back(_data);
            _data.reset();
            _hdrArray.clear();
        }

        if (_data.m_data.size() > _data.m_hdr.size) {
            QuasarAppUtils::Params::log("Invalid Package received. ",
                                        QuasarAppUtils::Warning);
            _data.reset();
            _hdrArray.clear();

        }
    }

    return true;
}
}
