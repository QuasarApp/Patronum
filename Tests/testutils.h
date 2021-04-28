#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <patronum.h>

class TestUtils
{
public:
    TestUtils();
    static bool wait(const bool &forWait, int msec);
    static bool wait(std::function<bool ()> forWait, int msec);

};

#endif // TESTUTILS_H
