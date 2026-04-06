#include <QtTest>
#include <QLineEdit>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void integerToBinary_zero()
    {
        MainWindow w;
        QCOMPARE(w.integerToBinary(0), QString("0"));
    }

    void integerToBinary_positive()
    {
        MainWindow w;
        QCOMPARE(w.integerToBinary(66), QString("1000010"));
    }

    void integerToBinary_large()
    {
        MainWindow w;
        QCOMPARE(w.integerToBinary(1024), QString("10000000000"));
    }

    void fractionalToBinary_half()
    {
        MainWindow w;
        QCOMPARE(w.fractionalToBinary(0.5, 10), QString("1"));
    }

    void fractionalToBinary_quarter()
    {
        MainWindow w;
        QCOMPARE(w.fractionalToBinary(0.25, 10), QString("01"));
    }

    void fractionalToBinary_tenth()
    {
        MainWindow w;
        QCOMPARE(w.fractionalToBinary(0.1, 10), QString("0001100110"));
    }

    void fractionalToBinary_depth_limit()
    {
        MainWindow w;
        QString result = w.fractionalToBinary(0.1, 5);
        QCOMPARE(result, QString("00011"));
    }

    void fractionalToBinary_zero()
    {
        MainWindow w;
        QCOMPARE(w.fractionalToBinary(0.0, 10), QString(""));
    }

    void fullConversion()
    {
        MainWindow w;
        double value = 3.14;
        int integerPart = static_cast<int>(value);
        double fractionalPart = value - integerPart;
        QString binaryInt = w.integerToBinary(integerPart);
        QString binaryFrac = w.fractionalToBinary(fractionalPart, 10);
        QString result = binaryInt;
        if (!binaryFrac.isEmpty())
            result += "." + binaryFrac;
        QCOMPARE(result, QString("11.0010001111"));
    }

    void invalidInput_emptyString()
    {
        MainWindow w;
        w.input->setText("");
        w.convert();
        QCOMPARE(w.result->text(), QString(""));
    }
};

QTEST_MAIN(TestMainWindow)
#include "test.moc"
