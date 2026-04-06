#include "test.h"
#include <QtTest>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

void TestMedians::medianOfThree()
{
    MainWindow w;
    QCOMPARE(w.medianOfThree(1, 5, 1), 1.0);
    QCOMPARE(w.medianOfThree(4, 5, 6), 5.0);
    QCOMPARE(w.medianOfThree(2, 1, 3), 2.0);
    QCOMPARE(w.medianOfThree(4, 4, 4), 4.0);
}

void TestMedians::averageOfTwo()
{
    MainWindow w;
    QCOMPARE(w.averageOfTwo(5, 7), 6.0);
    QCOMPARE(w.averageOfTwo(1, 2), 1.5);
    QCOMPARE(w.averageOfTwo(0, 0), 0.0);
}

void TestMedians::parseInput_valid()
{
    MainWindow w;
    QVector<int> result = w.parseInput("1,5,1,4,5,6");
    QVector<int> expected = {1,5,1,4,5,6};
    QCOMPARE(result, expected);
}

void TestMedians::parseInput_empty()
{
    MainWindow w;
    QVector<int> result = w.parseInput("");
    QVERIFY(result.isEmpty());
}

void TestMedians::parseInput_invalid()
{
    MainWindow w;
    QVector<int> result = w.parseInput("1,2,abc,3");
    QVector<int> expected = {1,2,3};
    QCOMPARE(result, expected);
}

void TestMedians::computeMedians_full()
{
    MainWindow w;
    w.inputEdit->setText("1,5,1,4,5,6,2,1,3,4,4,4,5,7");
    w.computeMedians();
    QString output = w.outputEdit->toPlainText();
    QCOMPARE(output, QString("Вектор b = { 1, 5, 2, 4, 6 }"));
}

void TestMedians::computeMedians_partial()
{
    MainWindow w;
    w.inputEdit->setText("1,2,3,4");
    w.computeMedians();
    QString output = w.outputEdit->toPlainText();
    // (1,2,3) -> 2, остаётся (4) -> 4
    QCOMPARE(output, QString("Вектор b = { 2, 4 }"));
}

void TestMedians::computeMedians_single()
{
    MainWindow w;
    w.inputEdit->setText("42");
    w.computeMedians();
    QString output = w.outputEdit->toPlainText();
    QCOMPARE(output, QString("Вектор b = { 42 }"));
}

void TestMedians::computeMedians_emptyInput()
{
    MainWindow w;
    w.inputEdit->setText("");
    // QTest::ignoreMessage(QtWarningMsg, "Введите последовательность чисел.");
    w.computeMedians();
    QVERIFY(w.outputEdit->toPlainText().isEmpty());
}

void TestMedians::computeMedians_invalidInput()
{
    MainWindow w;
    w.inputEdit->setText("1,2,abc");
    // QTest::ignoreMessage(QtWarningMsg, "Не удалось распознать числа.");
    w.computeMedians();
    // parseInput пропустит некорректные, останется [1,2] – два элемента -> среднее 1.5
    QString output = w.outputEdit->toPlainText();
    QCOMPARE(output, QString("Вектор b = { 1.5 }"));
}

QTEST_MAIN(TestMedians)
