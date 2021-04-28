#include <QtTest>

#include <QCoreApplication>
#include "defaultcontroller.h"
#include "defaultservice.h"
#include "testutils.h"

#define TEST_LOCAL_HOST "127.0.0.1"
#define TEST_PORT 27777

class testPatronum : public QObject
{
    Q_OBJECT

private:


public:
    testPatronum();

    void testPing();
    void testRandomCommad();

    ~testPatronum();

private slots:
    void initTestCase();
    void connectTest();



};

testPatronum::testPatronum() {
    QuasarAppUtils::Params::setArg("verbose", "3");

    QFile::remove("/tmp/PTestPatronum");

}

void testPatronum::testPing() {
    TestUtils utils;

    const char* arg[] = {
        "/",
        "ping"
    };
    DefaultController cli;
    QuasarAppUtils::Params::clearParsedData();
    QVERIFY(cli.send(2 , const_cast<char**>(arg)));
    QVERIFY(utils.wait([&cli](){return cli.isFinished();}, 1000));
    QVERIFY(cli.getResponce().value("Result") == "pong");
}

void testPatronum::testRandomCommad() {
    TestUtils utils;

    const char* arg[] = {
        "/",
        "fd"
    };
    DefaultController cli;
    QuasarAppUtils::Params::clearParsedData();

    QVERIFY(cli.send(2 , const_cast<char**>(arg)));
    QVERIFY(utils.wait([&cli](){return cli.isFinished();}, 1000));
    QVERIFY(cli.getResponce().contains("Error"));
}


void testPatronum::connectTest() {
    QCoreApplication::setApplicationName("PatronumTest");
    DefaultService serv;

    QTimer::singleShot(10, this, [this]() {
        testRandomCommad();
        testPing();

        QCoreApplication::exit(0);
    });

    QuasarAppUtils::Params::parseParams({"s"});

    QVERIFY(serv.exec() == 0);

}

testPatronum::~testPatronum() {

}

void testPatronum::initTestCase() {
}

QTEST_APPLESS_MAIN(testPatronum)

#include "tst_unittests.moc"
