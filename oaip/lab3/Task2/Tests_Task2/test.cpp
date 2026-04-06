#include "test.h"
#include <QtTest>
#include <QLineEdit>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

void TestAckermann::ackermann_0_0()
{
    MainWindow w;
    QCOMPARE(w.ackermann(0, 0), 1ULL);
}

void TestAckermann::ackermann_0_5()
{
    MainWindow w;
    QCOMPARE(w.ackermann(0, 5), 6ULL);
}

void TestAckermann::ackermann_1_0()
{
    MainWindow w;
    QCOMPARE(w.ackermann(1, 0), 2ULL);
}

void TestAckermann::ackermann_1_3()
{
    MainWindow w;
    QCOMPARE(w.ackermann(1, 3), 5ULL);
}

void TestAckermann::ackermann_2_2()
{
    MainWindow w;
    QCOMPARE(w.ackermann(2, 2), 7ULL);
}

void TestAckermann::ackermann_2_5()
{
    MainWindow w;
    QCOMPARE(w.ackermann(2, 5), 13ULL);
}

void TestAckermann::ackermann_3_1()
{
    MainWindow w;
    QCOMPARE(w.ackermann(3, 1), 13ULL);
}

void TestAckermann::ackermann_3_2()
{
    MainWindow w;
    QCOMPARE(w.ackermann(3, 2), 29ULL);
}

void TestAckermann::ackermann_4_0()
{
    MainWindow w;
    QCOMPARE(w.ackermann(4, 0), 13ULL);
}

void TestAckermann::invalid_input()
{
    MainWindow w;
    w.mEdit->setText("");
    w.nEdit->setText("");
    w.compute();
    QCOMPARE(w.resultLabel->text(), QString(""));
}

QTEST_MAIN(TestAckermann)
