#include "test.h"
#include <QtTest>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

void TestInterpolation::interpolationSort_known()
{
    MainWindow w;
    w.arrayEdit->setText("9 5 2 8 1");
    w.interpolationSort();
    QVector<int> expected = {1, 2, 5, 8, 9};
    QCOMPARE(w.sortedArray, expected);
    QString output = w.outputEdit->toPlainText();
    QVERIFY(output.contains("Результат: 1 2 5 8 9"));
}

void TestInterpolation::interpolationSort_empty()
{
    MainWindow w;
    w.arrayEdit->setText("");
    w.interpolationSort();
    QVERIFY(w.sortedArray.isEmpty());
    QCOMPARE(w.outputEdit->toPlainText(), QString());
}

void TestInterpolation::interpolationSort_alreadySorted()
{
    MainWindow w;
    w.arrayEdit->setText("1 2 3 4 5");
    w.interpolationSort();
    QVector<int> expected = {1, 2, 3, 4, 5};
    QCOMPARE(w.sortedArray, expected);
}

void TestInterpolation::binpow_basic()
{
    MainWindow w;
    int result = w.binpow(2, 10, 1000);
    QCOMPARE(result, 24); // 2^10 = 1024, mod 1000 = 24
}

void TestInterpolation::binpow_mod1()
{
    MainWindow w;
    int result = w.binpow(5, 100, 1);
    QCOMPARE(result, 0);
}

void TestInterpolation::binpow_zeroExponent()
{
    MainWindow w;
    int result = w.binpow(7, 0, 100);
    QCOMPARE(result, 1);
}

void TestInterpolation::binarySearchAndPow_valid()
{
    MainWindow w;
    w.sortedArray = {1, 2, 5, 8, 9};
    w.searchEdit->setText("5");
    w.modEdit->setText("1000");
    w.binarySearchAndPow();
    QCOMPARE(w.resultLabel->text(),
             QString("Индекс найденного элемента: 2\nДлина массива: 5\n(2 ^ 5) mod 1000 = 32"));
}

void TestInterpolation::binarySearchAndPow_notFound()
{
    MainWindow w;
    w.sortedArray = {1, 2, 5, 8, 9};
    w.searchEdit->setText("7");
    w.modEdit->setText("1000");
    w.binarySearchAndPow();
    QCOMPARE(w.resultLabel->text(), QString("Число 7 не найдено в массиве."));
}

void TestInterpolation::binarySearchAndPow_invalidInput()
{
    MainWindow w;
    w.sortedArray = {1, 2, 5, 8, 9};
    w.searchEdit->setText("abc");
    w.modEdit->setText("1000");
    w.binarySearchAndPow();
    QCOMPARE(w.resultLabel->text(), QString());
}

void TestInterpolation::generateRandom()
{
    MainWindow w;
    w.sizeSpin->setValue(5);
    w.generateRandom();
    QCOMPARE(w.originalArray.size(), 5);
    QString text = w.arrayEdit->text();
    QStringList parts = text.split(' ', Qt::SkipEmptyParts);
    QCOMPARE(parts.size(), 5);
    QVERIFY(w.outputEdit->toPlainText().contains("Сгенерирован массив"));
    QVERIFY(w.sortedArray.isEmpty());
    QVERIFY(w.resultLabel->text().isEmpty());
}

QTEST_MAIN(TestInterpolation)
