#include "test_date.h"
#include <stdexcept>

void TestDate::testConstructorValid()
{
    Date d(1, 1, 2025);
    QCOMPARE(d.getDay(), 1);
    QCOMPARE(d.getMonth(), 1);
    QCOMPARE(d.getYear(), 2025);
}

void TestDate::testConstructorInvalidDay()
{
    QVERIFY_EXCEPTION_THROWN(Date(31, 2, 2025), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(Date(29, 2, 2023), std::invalid_argument);
}

void TestDate::testConstructorInvalidMonth()
{
    QVERIFY_EXCEPTION_THROWN(Date(1, 13, 2025), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(Date(1, 0, 2025), std::invalid_argument);
}

void TestDate::testNextDay_ordinary()
{
    Date d(15, 3, 2025);
    Date next = d.nextDay();
    QCOMPARE(next.getDay(), 16);
    QCOMPARE(next.getMonth(), 3);
    QCOMPARE(next.getYear(), 2025);
}

void TestDate::testNextDay_endOfMonth()
{
    Date d(31, 1, 2025);
    Date next = d.nextDay();
    QCOMPARE(next.getDay(), 1);
    QCOMPARE(next.getMonth(), 2);
    QCOMPARE(next.getYear(), 2025);
}

void TestDate::testNextDay_endOfYear()
{
    Date d(31, 12, 2025);
    Date next = d.nextDay();
    QCOMPARE(next.getDay(), 1);
    QCOMPARE(next.getMonth(), 1);
    QCOMPARE(next.getYear(), 2026);
}

void TestDate::testPreviousDay()
{
    Date d(1, 3, 2025);
    Date prev = d.previousDay();
    QCOMPARE(prev.getDay(), 28);
    QCOMPARE(prev.getMonth(), 2);
    QCOMPARE(prev.getYear(), 2025);
}

void TestDate::testIsLeap()
{
    QVERIFY(Date(1,1,2020).isLeap() == true);
    QVERIFY(Date(1,1,2021).isLeap() == false);
    QVERIFY(Date(1,1,1900).isLeap() == false);
    QVERIFY(Date(1,1,2000).isLeap() == true);
}

void TestDate::testWeekNumber_range()
{
    short wn = Date(31,12,2024).weekNumber();
    QVERIFY(wn >= 1 && wn <= 53);
}

void TestDate::testDaysTillYourBirthday()
{
    Date today(1, 1, 2025);
    Date birthday(15, 6, 2030);
    int days = today.daysTillYourBirthday(birthday);
    QCOMPARE(days, 1991);   // исправлено
}

void TestDate::testDuration()
{
    Date d1(1, 1, 2025);
    Date d2(1, 1, 2026);
    QCOMPARE(d2.duration(d1), 365);
    QCOMPARE(d1.duration(d2), -365);
}

void TestDate::testToString()
{
    Date d(5, 7, 2025);
    QCOMPARE(QString::fromStdString(d.toString()), QString("05.07.2025"));
}

void TestDate::testOperators()
{
    Date d1(1, 1, 2025);
    Date d2(2, 1, 2025);
    QVERIFY(d1 < d2);
    QVERIFY(!(d2 < d1));
    QVERIFY(d1 == d1);
    QVERIFY(!(d1 == d2));
}

int main(int argc, char *argv[])
{
    TestDate test;
    return QTest::qExec(&test, argc, argv);
}
