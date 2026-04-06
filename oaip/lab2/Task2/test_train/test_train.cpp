#include "test_train.h"
#include <QTest>
#include <stdexcept>

void TestTrain::testConstructorWithParams()
{
    QDate date(2025, 4, 1);
    QTime time(12, 0);
    Train t(date, 101, "Минск", time, 20, 30, 10, 15);

    QCOMPARE(t.getDate(), date);
    QCOMPARE(t.getNumber(), 101);
    QCOMPARE(t.getDestination(), QString("Минск"));
    QCOMPARE(t.getTime(), time);
    QCOMPARE(t.getTotalCoupe(), 20);
    QCOMPARE(t.getTotalPlatzkart(), 30);
    QCOMPARE(t.getFreeCoupe(), 10);
    QCOMPARE(t.getFreePlatzkart(), 15);
}

void TestTrain::testDefaultConstructor()
{
    Train t;
    QCOMPARE(t.getDate(), QDate(1, 1, 2000));
    QCOMPARE(t.getNumber(), 0);
    QCOMPARE(t.getDestination(), QString());
    QCOMPARE(t.getTime(), QTime());
    QCOMPARE(t.getTotalCoupe(), 0);
    QCOMPARE(t.getTotalPlatzkart(), 0);
    QCOMPARE(t.getFreeCoupe(), 0);
    QCOMPARE(t.getFreePlatzkart(), 0);
}

void TestTrain::testComputedFields()
{
    Train t(QDate(2025,4,1), 101, "Минск", QTime(12,0), 20, 30, 10, 15);
    QCOMPARE(t.soldCoupe(), 10);
    QCOMPARE(t.soldPlatzkart(), 15);
    QCOMPARE(t.totalFree(), 25);
}

void TestTrain::testSetters()
{
    Train t;
    QDate date(2025, 5, 1);
    QTime time(14, 30);
    t.setDate(date);
    t.setNumber(202);
    t.setDestination("Гомель");
    t.setTime(time);
    t.setTotalCoupe(25);
    t.setTotalPlatzkart(35);
    t.setFreeCoupe(12);
    t.setFreePlatzkart(20);

    QCOMPARE(t.getDate(), date);
    QCOMPARE(t.getNumber(), 202);
    QCOMPARE(t.getDestination(), QString("Гомель"));
    QCOMPARE(t.getTime(), time);
    QCOMPARE(t.getTotalCoupe(), 25);
    QCOMPARE(t.getTotalPlatzkart(), 35);
    QCOMPARE(t.getFreeCoupe(), 12);
    QCOMPARE(t.getFreePlatzkart(), 20);
}

void TestTrain::testSerialization()
{
    Train original(QDate(2025,4,1), 101, "Минск", QTime(12,0), 20, 30, 10, 15);
    QString str = original.toString();
    Train restored = Train::fromString(str);

    QCOMPARE(restored.getDate(), original.getDate());
    QCOMPARE(restored.getNumber(), original.getNumber());
    QCOMPARE(restored.getDestination(), original.getDestination());
    QCOMPARE(restored.getTime(), original.getTime());
    QCOMPARE(restored.getTotalCoupe(), original.getTotalCoupe());
    QCOMPARE(restored.getTotalPlatzkart(), original.getTotalPlatzkart());
    QCOMPARE(restored.getFreeCoupe(), original.getFreeCoupe());
    QCOMPARE(restored.getFreePlatzkart(), original.getFreePlatzkart());
}

void TestTrain::testFromStringInvalid()
{
    QString badLine = "01.04.2025|101|Минск|12:00|20|30|10";
    QVERIFY_EXCEPTION_THROWN(Train::fromString(badLine), std::invalid_argument);

    QString badNumber = "01.04.2025|abc|Минск|12:00|20|30|10|15";
    QVERIFY_EXCEPTION_THROWN(Train::fromString(badNumber), std::invalid_argument);
}

void TestTrain::testLessThanOperator()
{
    Train t1(QDate(2025,4,1), 101, "Минск", QTime(12,0), 20,30,10,15);
    Train t2(QDate(2025,4,2), 101, "Минск", QTime(12,0), 20,30,10,15);
    Train t3(QDate(2025,4,1), 102, "Минск", QTime(12,0), 20,30,10,15);
    Train t4(QDate(2025,4,1), 101, "Минск", QTime(13,0), 20,30,10,15);

    QVERIFY(t1 < t2);
    QVERIFY(t1 < t3);
    QVERIFY(t1 < t4);
    QVERIFY(!(t2 < t1));
    QVERIFY(!(t3 < t1));
    QVERIFY(!(t4 < t1));
}

void TestTrain::testLeapYearDate()
{
    QDate date(2024, 2, 29);
    Train t(date, 100, "Москва", QTime(10,0), 10,10,5,5);
    QCOMPARE(t.getDate(), date);
}

void TestTrain::testTime24Hour()
{
    QTime time(23, 45);
    Train t(QDate(2025,1,1), 100, "Киев", time, 10,10,5,5);
    QCOMPARE(t.getTime().toString("hh:mm"), QString("23:45"));
}

void TestTrain::testNegativeSeats()
{
    Train t(QDate(2025,1,1), 100, "Киев", QTime(10,0), -5, -3, -2, -1);
    QCOMPARE(t.getTotalCoupe(), -5);
    QCOMPARE(t.getTotalPlatzkart(), -3);
    QCOMPARE(t.getFreeCoupe(), -2);
    QCOMPARE(t.getFreePlatzkart(), -1);
}

QTEST_MAIN(TestTrain)
