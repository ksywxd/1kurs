#include "test.h"
#include <QtTest>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

void TestSorting::heapSort()
{
    MainWindow w;
    QVector<int> arr = {5, 2, 8, 1, 9};
    QVector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    int* data = arr.data();
    w.heapSort(data, arr.size());
    for (int i = 0; i < arr.size(); ++i) {
        QCOMPARE(arr[i], expected[i]);
    }
}

void TestSorting::quickSort()
{
    MainWindow w;
    QVector<int> arr = {5, 2, 8, 1, 9};
    QVector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    int* data = arr.data();
    w.quickSort(data, 0, arr.size() - 1);
    for (int i = 0; i < arr.size(); ++i) {
        QCOMPARE(arr[i], expected[i]);
    }
}

void TestSorting::mergeSort()
{
    MainWindow w;
    QVector<int> arr = {5, 2, 8, 1, 9};
    QVector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    int* data = arr.data();
    w.mergeSort(data, 0, arr.size() - 1);
    for (int i = 0; i < arr.size(); ++i) {
        QCOMPARE(arr[i], expected[i]);
    }
}

void TestSorting::binarySearchFound()
{
    MainWindow w;
    QVector<int> sorted = {1, 2, 5, 8, 9};
    int idx = w.binarySearch(sorted.data(), sorted.size(), 5);
    QCOMPARE(idx, 2);
}

void TestSorting::binarySearchNotFound()
{
    MainWindow w;
    QVector<int> sorted = {1, 2, 5, 8, 9};
    int idx = w.binarySearch(sorted.data(), sorted.size(), 7);
    QCOMPARE(idx, -1);
}

void TestSorting::sortEmptyArray()
{
    MainWindow w;
    w.originalArray.clear();
    w.sortHeap();
    w.sortQuick();
    w.sortMerge();
    QVERIFY(w.originalArray.isEmpty());
    QCOMPARE(w.resultEdit->toPlainText(), QString());
}

void TestSorting::binarySearchEmptyArray()
{
    MainWindow w;
    w.originalArray.clear();
    w.binarySearch();
    QCOMPARE(w.searchResultLabel->text(), QString());
}

void TestSorting::generateRandom()
{
    MainWindow w;
    w.sizeSpin->setValue(5);
    w.generateRandom();
    QCOMPARE(w.originalArray.size(), 5);
    QString text = w.arrayEdit->text();
    QStringList parts = text.split(' ', Qt::SkipEmptyParts);
    QCOMPARE(parts.size(), 5);
}

void TestSorting::binarySearchManualInput()
{
    MainWindow w;
    w.originalArray = {10, 20, 30, 40, 50};
    w.arrayEdit->setText("10 20 30 40 50");
    w.searchEdit->setText("30");
    w.binarySearch();
    QCOMPARE(w.searchResultLabel->text(), QString("Число 30 найдено на позиции 2 (в отсортированном массиве)."));
}

void TestSorting::sortManualInput()
{
    MainWindow w;
    w.originalArray = {9, 5, 2, 8, 1};
    w.arrayEdit->setText("9 5 2 8 1");
    w.sortQuick();
    QString output = w.resultEdit->toPlainText();
    QVERIFY(output.contains("Quick sort result: 1 2 5 8 9"));
}

QTEST_MAIN(TestSorting)
