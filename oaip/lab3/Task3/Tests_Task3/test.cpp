#include "test.h"
#include <QtTest>
#include <QLineEdit>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

void TestReverseNumber::reverse_123()
{
    MainWindow w;
    w.input->setText("123");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString("Перевёрнутое число: 321"));
}

void TestReverseNumber::reverse_972()
{
    MainWindow w;
    w.input->setText("972");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString("Перевёрнутое число: 279"));
}

void TestReverseNumber::reverse_5()
{
    MainWindow w;
    w.input->setText("5");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString("Перевёрнутое число: 5"));
}

void TestReverseNumber::reverse_1001_hasZero()
{
    MainWindow w;
    w.input->setText("1001");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString(""));
}

void TestReverseNumber::reverse_120_hasZero()
{
    MainWindow w;
    w.input->setText("120");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString(""));
}

void TestReverseNumber::reverse_negative()
{
    MainWindow w;
    w.input->setText("-123");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString(""));
}

void TestReverseNumber::reverse_nonNumeric()
{
    MainWindow w;
    w.input->setText("abc");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString(""));
}

void TestReverseNumber::reverse_zero()
{
    MainWindow w;
    w.input->setText("0");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString(""));
}

void TestReverseNumber::reverse_large()
{
    MainWindow w;
    w.input->setText("987654321");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString("Перевёрнутое число: 123456789"));
}

void TestReverseNumber::invalid_input_empty()
{
    MainWindow w;
    w.input->setText("");
    w.reverseNumber();
    QCOMPARE(w.result->text(), QString(""));
}

QTEST_MAIN(TestReverseNumber)
