#ifndef TEST_DATE_H
#define TEST_DATE_H

#include <QObject>
#include <QTest>
#include "date.h"

class TestDate : public QObject
{
    Q_OBJECT

private slots:
    void testConstructorValid();
    void testConstructorInvalidDay();
    void testConstructorInvalidMonth();
    void testNextDay_ordinary();
    void testNextDay_endOfMonth();
    void testNextDay_endOfYear();
    void testPreviousDay();
    void testIsLeap();
    void testWeekNumber_range();
    void testDaysTillYourBirthday();
    void testDuration();
    void testToString();
    void testOperators();
};

#endif // TEST_DATE_H
