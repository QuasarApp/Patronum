#include <QtTest>

#include <QCoreApplication>
#include "defaultcontroller.h"
#include "defaultservice.h"
#include "testutils.h"

#define TEST_LOCAL_HOST "127.0.0.1"
#define TEST_PORT 27777

class testProtockol : public QObject
{
    Q_OBJECT

private:


public:
    testProtockol();


    ~testProtockol();

private slots:
    void initTestCase();
    void connectTest();



};

testProtockol::testProtockol() {
    QuasarAppUtils::Params::setArg("verbose", 3);

}

void testProtockol::connectTest() {
    DefaultService serv;

    QTimer::singleShot(0, [](){
        const char* arg[] = {
            "/",
            "fd"
        };
        DefaultController cli;

        QVERIFY(cli.send(2 , arg));
        QVERIFY(cli.waitForResponce(1000));
        QVERIFY(cli.getResponce().value("Result") == "pong");
    });


    QVERIFY(serv.exec() == 0);

}

testProtockol::~testProtockol() {

}

void testProtockol::initTestCase() {
}

QTEST_APPLESS_MAIN(testProtockol)

#include "tst_unittests.moc"
