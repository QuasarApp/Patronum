#ifndef PACKAGE_H
#define PACKAGE_H

#include <QDataStream>

namespace Patronum {

enum class Command: char {
    FeaturesRequest,
    Features,
    Feature
};

struct Package
{
    Command cmd;
    char* data;
};

}

#endif // PACKAGE_H
