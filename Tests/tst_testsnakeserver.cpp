#include <QtTest>

#include <quasarapp.h>
#include <QCoreApplication>
#include "testutils.h"

#include <patronum.h>

#define TEST_LOCAL_HOST "127.0.0.1"
#define TEST_PORT 27777

class testProtockol : public QObject
{
    Q_OBJECT

private:


public:
    testProtockol();

    void connectTest(Patronum::Service *service, Patronum::Controller *terminal);

    ~testProtockol();

private slots:
    void initTestCase();
    void testPakageData();
    void testBaseNode();
    void testUser();


};

testProtockol::testProtockol() {
    QuasarAppUtils::Params::setArg("verbose", 3);

}

void testProtockol::connectTest(NP::Client *cli, NP::BaseNode *serv) {
    QVERIFY(serv->run(TEST_LOCAL_HOST, TEST_PORT));
    QVERIFY(TestUtils::connectFunc(cli, TEST_LOCAL_HOST, TEST_PORT));
}

testProtockol::~testProtockol() {

}

void testProtockol::initTestCase() {
}

QTEST_APPLESS_MAIN(testProtockol)

#include "tst_testsnakeserver.moc"
